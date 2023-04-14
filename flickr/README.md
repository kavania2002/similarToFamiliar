# Flickr Dataset

## Approach
- Used Following Flickr APIs in [index.py](/flickr/index.py)

    ### Groups Output

    - Code
    - `resp = flickr.groups.search(text=topic, per_page=500)`
    - `resp['groups']['group']`
    - Output
    - `[{'nsid': '571346@N22', 'name': 'soul nature', 'eighteenplus': 0, 'iconserver': '2033', 'iconfarm': 3, 'members': '6', 'pool_count': '25', 'topic_count': '0', 'privacy': '3'}, {'nsid': '895927@N24', 'name': 'Nature détournée', 'eighteenplus': 0, 'iconserver': '3065', 'iconfarm': 4, 'members': '34', 'pool_count': '44', 'topic_count': '0', 'privacy': '3'}]`

    ### Members Output

    - Code
    - `members = flickr.groups.members.getList(group_id=resp['groups']['group'][0]['nsid'], per_page=500)`
    - Output
    - `{'members': {'page': 1, 'pages': 275, 'perpage': 100, 'total': '27460', 'member': [{'nsid': '86603453@N00', 'username': 'nightofforce', 'iconserver': '195', 'iconfarm': 1, 'membertype': '2', 'realname': ''}, {'nsid': '195722094@N03', 'username': 'sasquatchphotog', 'iconserver': '65535', 'iconfarm': 66, 'membertype': '2'}, {'nsid': '196155068@N05', 'username': 'stanleyweyman8', 'iconserver': '65535', 'iconfarm': 66, 'membertype': '2', 'realname': 'Stanley Weyman'}]}, 'stat': 'ok'}`

    ### Following Output

    - Code
    - `following = flickr.contacts.getPublicList(user_id="42652867@N00")`
    - Output
    - `{'contacts': {'page': 1, 'pages': 1, 'per_page': 1000, 'perpage': 1000, 'total': 10, 'contact': [{'nsid': '56136622@N00', 'username': 'Ansh!!!', 'iconserver': '0', 'iconfarm': 0, 'ignored': 0, 'rev_ignored': 0}, {'nsid': '7767807@N04', 'username': 'debrajdutta78', 'iconserver': '0', 'iconfarm': 0, 'ignored': 0, 'rev_ignored': 0}, {'nsid': '60221201@N00', 'username': 'vaishnav712', 'iconserver': '0', 'iconfarm': 0, 'ignored': 0, 'rev_ignored': 0}]}, 'stat': 'ok'}`

<br>

- Used [main.cpp](/flickr/main.cpp) to analyse the data and plot the graph using [cppPlotter.ipynb](/flickr/cppPlotter.ipynb)

- You can see the results here: [category-wise](/flickr/category-wise/)


## Usage
### Fetching the Flickr Data

- First of all, get the Flickr API credentials from following [site](https://www.flickr.com/services/api/)

- Create .env file and write data in the following format
    ```
    API_KEY=<FLICKR_API>
    API_SECRET=<FLICKR SECRET>
    ```

- Run the [index.py](/flickr/index.py) and enter the topic you want to search for and the number of groups as asked.

### Obtaining the data
The process requires having 2 datasets, a list of edges, and a list of friends. Preferably, it is stored in the format as ```edges{file_suffix}.csv``` and ```friends{file_suffix}.csv``` in the same directory as the ```main.cpp``` file.


### Compiling the C++ data processor
#### On Unix-like OS (linux/macOS)

In the directory of main.cpp, execute

```bash
g++ main.cpp -o main
```

then to run the compiled binary, execute the following

```bash
./main ${file_suffix}
```

Here, add the suffix, the following files have in the format ```edges{file_suffix}.csv``` and ```friends{file_suffix}.csv```
for example, ```edges15.csv``` has the suffix ```15```.

When executing, there will be additional intermediate files generated and stored in same directory to make it run faster next time.
