--------------------------------------------------------Que6---------------------------------------------


from cassandra.cluster import Cluster
from cassandra.query import SimpleStatement
import json
import os
from os import listdir
from pprint import pprint

KEYSPACE="key_150101062"

def main():
    cluster = Cluster(['127.0.0.1'])
    session = cluster.connect()
    # session.execute('DROP KEYSPACE %s' %KEYSPACE)
    session.execute('CREATE KEYSPACE %s WITH REPLICATION = { \'class\' : \'SimpleStrategy\', \'replication_factor\' : 1 }' %KEYSPACE)
    session.set_keyspace(KEYSPACE)

    # session.execute('drop table testkeyspace.que6')

    session.execute("""
        CREATE TABLE que6 (
            location text,
            date_ text,
            tid_count int,
            PRIMARY KEY (location,date_,tid_count)   
        )
        """)

    query = SimpleStatement("""
        INSERT INTO que6 ( \
            location, \
            date_, \
            tid_count)
        VALUES (%s, %s, %s)
        """)
    new_dict = {}
    finaldic = {}
    for x in os.listdir('./workshop_dataset1'):
        if x.endswith('.json'):
            new_dict[x[:-5]]={}
            y = './workshop_dataset1/' + x
            data = json.load(open(y))
            for key,value in data.items():
                if data[key]['location']:
                    finaldic[data[key]['location']] = {}
                    if new_dict[data[key]['date']].get(data[key]['location']) is None:
                        new_dict[data[key]['date']][data[key]['location']] = 1
                    else:
                        new_dict[data[key]['date']][data[key]['location']] += 1

    for x in new_dict:
        for y,z in new_dict[x].items():
            session.execute(query,(y,x,z))
            
if __name__ == "__main__":
	main()



---------------------------------------------------------Que11-------------------------------------------------------------------

from cassandra.cluster import Cluster
from cassandra.query import SimpleStatement
import json
import os
from os import listdir
from pprint import pprint

KEYSPACE="key_150101062"

def main():
    cluster = Cluster(['127.0.0.1'])
    session = cluster.connect()
    # session.execute('DROP KEYSPACE %s' %KEYSPACE)
    # session.execute('CREATE KEYSPACE %s WITH REPLICATION = { \'class\' : \'SimpleStrategy\', \'replication_factor\' : 1 }' %KEYSPACE)
    session.set_keyspace(KEYSPACE)

    # session.execute('drop table testkeyspace.que11')

    session.execute("""
        CREATE TABLE que11 (
            date_ text,
            mention text,
            location text,
            freq int,
            PRIMARY KEY (date_,(mention,location),tid_count)   
        )
        """)

    query = SimpleStatement("""
        INSERT INTO que11 ( \
            date_, \
            mention, \
            location, \
            freq)
        VALUES (%s, %s, %s, %s)
        """)
    new_dict = {}
    for x in os.listdir('./workshop_dataset1'):
        if x.endswith('.json'):
            new_dict[x[:-5]]={}
            y = './workshop_dataset1/' + x
            data = json.load(open(y))
            for key,value in data.items():
                for w in data[key]['mentions']:

if __name__ == "__main__":
    main()