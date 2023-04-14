# Flickr APIs

## Groups Output

- Code
  - `resp = flickr.groups.search(text=topic, per_page=500)`
  - `resp['groups']['group']`
- Output
  - `[{'nsid': '571346@N22', 'name': 'soul nature', 'eighteenplus': 0, 'iconserver': '2033', 'iconfarm': 3, 'members': '6', 'pool_count': '25', 'topic_count': '0', 'privacy': '3'}, {'nsid': '895927@N24', 'name': 'Nature détournée', 'eighteenplus': 0, 'iconserver': '3065', 'iconfarm': 4, 'members': '34', 'pool_count': '44', 'topic_count': '0', 'privacy': '3'}]`

## Members Output

- Code
  - `members = flickr.groups.members.getList(group_id=resp['groups']['group'][0]['nsid'], per_page=500)`
- Output
  - `{'members': {'page': 1, 'pages': 275, 'perpage': 100, 'total': '27460', 'member': [{'nsid': '86603453@N00', 'username': 'nightofforce', 'iconserver': '195', 'iconfarm': 1, 'membertype': '2', 'realname': ''}, {'nsid': '195722094@N03', 'username': 'sasquatchphotog', 'iconserver': '65535', 'iconfarm': 66, 'membertype': '2'}, {'nsid': '196155068@N05', 'username': 'stanleyweyman8', 'iconserver': '65535', 'iconfarm': 66, 'membertype': '2', 'realname': 'Stanley Weyman'}]}, 'stat': 'ok'}`

## Following Output

- Code
  - `following = flickr.contacts.getPublicList(user_id="42652867@N00")`
- Output
  - `{'contacts': {'page': 1, 'pages': 1, 'per_page': 1000, 'perpage': 1000, 'total': 10, 'contact': [{'nsid': '56136622@N00', 'username': 'Ansh!!!', 'iconserver': '0', 'iconfarm': 0, 'ignored': 0, 'rev_ignored': 0}, {'nsid': '7767807@N04', 'username': 'debrajdutta78', 'iconserver': '0', 'iconfarm': 0, 'ignored': 0, 'rev_ignored': 0}, {'nsid': '60221201@N00', 'username': 'vaishnav712', 'iconserver': '0', 'iconfarm': 0, 'ignored': 0, 'rev_ignored': 0}]}, 'stat': 'ok'}`
