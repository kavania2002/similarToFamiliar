import flickrapi
import webbrowser
import json
import csv
import os
from dotenv import load_dotenv

load_dotenv()

# loading the Flick API Keys from .env file
API_KEY = os.getenv('API_KEY')
API_SECRET = os.getenv('API_SECRET')

api_key = API_KEY
api_secret = API_SECRET

flickr = flickrapi.FlickrAPI(api_key, api_secret, format='parsed-json')

# Finding Groups related to Topic
topic = input("Enter The Topic You want to Search For: ")
maxGroups = int(input("Enter the Maximum number of Groups to search for: "))


def authentication():

    # Referenced from https://stuvel.eu/flickrapi-doc/3-auth.html#authenticating-without-local-web-server
    # Only do this if we don't have a valid token already
    if not flickr.token_valid(perms='read'):

        # Get a request token
        flickr.get_request_token(oauth_callback='oob')

        # Open a browser at the authentication URL. Do this however
        # you want, as long as the user visits that URL.
        authorize_url = flickr.auth_url(perms='read')
        webbrowser.open_new_tab(authorize_url)

        # Get the verifier code from the user. Do this however you
        # want, as long as the user gives the application the code.
        verifier = str(input('Verifier code: '))

        # Trade the request token for an access token
        flickr.get_access_token(verifier)


def groupMembers():
    resp = flickr.groups.search(text=topic)

    print(f"Total Number of Groups: {resp['groups']['total']}")
    print(f"Total Number of Pages: {resp['groups']['pages']}")
    print(
        f"Therefore we will go for only {maxGroups} groups for now with 1000 - 2000 members...")

    groupCount = 0
    groupIDs = []

    uniqueMembers = {}

    for groupPage in range(1, resp['groups']['pages']+1):
        groupsResponse = flickr.groups.search(
            text=topic, per_page=500, page=groupPage)

        for group in groupsResponse['groups']['group']:
            if (int(group['members']) >= 1000 and int(group['members']) <= 2000):
                groupIDs.append(group['nsid'])
                groupCount += 1

                if (groupCount >= maxGroups):
                    break
        if (groupCount >= maxGroups):
            break

    # Form an edge between the member within same group
    edges = open(f'./dataset/{topic}/edges{maxGroups}.csv',
                 'w', newline='', encoding='utf-8')
    edgeWriter = csv.writer(edges)
    edgeWriter.writerow(["Source", "Target"])

    nodes = open(f'./dataset/{topic}/nodes{maxGroups}.csv',
                 'w', newline='', encoding='utf-8')
    nodeWriter = csv.writer(nodes)
    nodeWriter.writerow(["Id", "Label"])

    groupNumber = 1
    for groupId in (groupIDs):
        # Get all the Members of the particular group
        memberList = flickr.groups.members.getList(
            group_id=groupId, per_page=500)
        if (memberList['stat'] == 'ok'):
            print(
                f"{groupNumber}. Group Id: {groupId}, Number of Pages: {memberList['members']['pages']}, Per Page: {memberList['members']['perpage']}")
            groupNumber += 1

            # Getting into each page to get all members
            allMembers = []
            for memberListPage in range(1, memberList['members']['pages']+1):
                members = flickr.groups.members.getList(
                    group_id=groupId, per_page=500, page=memberListPage)
                if (members['stat'] == 'ok'):
                    print(
                        f"Page: {memberListPage}, Per Page: {members['members']['perpage']}")
                    for member in members['members']['member']:
                        if (uniqueMembers.get(member['nsid']) == None):
                            nodeWriter.writerow(
                                [member['nsid'], member['username']])
                            uniqueMembers[member['nsid']] = member['username']
                        allMembers.append(member['nsid'])

            print('Number of Members Before Writing: ', len(allMembers))
            for i in range(len(allMembers)):
                for j in range(i+1, len(allMembers)):
                    data = [
                        allMembers[i], allMembers[j]]
                    edgeWriter.writerow(data)
            print(f"Number of Members: {memberList['members']['total']}")

    edges.close()
    nodes.close()


def findFollowing():
    friends = open(
        f'./dataset/{topic}/friends{maxGroups}.csv', 'w', newline='', encoding='utf-8')
    friendsWriter = csv.writer(friends)

    membersFile = open(
        f'./dataset/{topic}/nodes{maxGroups}.csv', 'r', newline='', encoding='utf-8')

    # creating blank error.txt file
    with open(f'./dataset/{topic}/error{maxGroups}.txt', 'w') as fp:
        pass

    membersList = csv.reader(membersFile, delimiter=',')
    for (userId, username) in membersList:

        try:
            # the first row contains Id.
            if (userId != 'Id'):
                # list of people the person is following
                following = flickr.contacts.getPublicList(user_id=userId)
                print(
                    f"Username: {username}\t Friends: {following['contacts']['total']}")

                for pageNumber in range(1, following['contacts']['pages']):
                    following = flickr.contacts.getPublicList(
                        user_id=userId, page=pageNumber)

                    for member in following['contacts']['contact']:
                        friendsWriter.writerow([userId, member['nsid']])
        except:
            with open(f'./dataset/{topic}/error{maxGroups}.txt', 'a', newline='') as file:
                file.write(f"{userId} made errors for us\n")
            print(f"{userId} made errors for us")

    friends.close()


if __name__ == "__main__":

    # create a folder dataset if not exists
    path = os.path.join(os.getcwd(), f'dataset/{topic}')
    os.makedirs(path, exist_ok=True)

    print("Step1: Authentication")
    authentication()

    print(f"Step2: Finding groups relating to '{topic}'")
    groupMembers()

    print("Step3: Find Followings of all Obtained People")
    findFollowing()
