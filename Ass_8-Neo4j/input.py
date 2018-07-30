import os
from py2neo import Graph, Node, Relationship
import json
import os
from os import listdir

def main():
	graph = Graph("http://localhost:7474/db/data/",user="neo4j",password="password")
	graph.run("CREATE INDEX ON :Tweet(tid)")
	graph.run("CREATE INDEX ON :Hashtag(hashtag)")
	graph.run("CREATE INDEX ON :User(author_screen_name)")
	
	data = json.load(open("dataset.json"))
	ctr = 0

	tx = graph.begin()

	stmt_user = "MERGE (u:User {author_screen_name:{asn}}) set u.author={auth},u.author_id={aid},u.author_image={aim}"
	stmt_tweet = "MERGE (t:Tweet {tid: {t_id}}) set t.location= {loc},t.type={type}"
	stmt_hash = "MERGE (h:Hashtag {hashtag:{hash}})"
	stmt_mention = "MERGE (u:User{author_screen_name:{asn}})"

	for key,value in data.items():
		tid = key.encode('ascii')

		tx.append(stmt_tweet,{"t_id":tid,"loc":data[key]['location'],"type":data[key]['type']})

		ctr += 5
		if ctr % 1000 == 0:
			tx.commit()
			tx = graph.begin()

		dic = {}
		if data[key]['hashtags'] is not None:
			for hashtag in data[key]['hashtags']:
				if hashtag:
					if dic.get(hashtag) is None:
						dic[hashtag] = True
						tx.append(stmt_hash,{"hash":hashtag})
						tx.append("MATCH (t:Tweet{tid:{t_id}}),(h:Hashtag{hashtag:{hash}}) CREATE (t)-[:Has_hashtag]->(h)",t_id=tid,hash=hashtag)

		dic = {}
		if data[key]['mentions'] is not None:
			for mention in data[key]['mentions']:
				if mention:
					if dic.get(mention) is None:
						dic[mention] = True
						tx.append(stmt_mention,{"asn":mention})
						tx.append("MATCH (t:Tweet{tid:{t_id}}),(u:User{author_screen_name:{mention}}) CREATE (t)-[:Mentions]->(u)",t_id=tid,mention=mention)

if __name__ == "__main__":
	main()