----------------------------------------------------------Que7-------------------------------------------------------------

<?php

require_once 'vendor/autoload.php';

use GraphAware\Neo4j\Client\ClientBuilder;

$client = ClientBuilder::create()
    ->addConnection('bolt', 'bolt://neo4j:password@localhost:7687')
    ->build();

$query7 = "match (u:User)<-[:Mentions]-(t:Tweet)-[:Has_hashtag]->(h:Hashtag) where h.hashtag="."'HappyBirthdayJustinBieber'"." and t.type="."'Tweet'"." return h.hashtag as hashtag, u.author_screen_name as mention, collect(t.tid) as list,size(collect(t.tid)) as cnt order by size(list) desc limit 3;
";

$result = $client->run($query7);

foreach ($result->getRecords() as $record) {
	    echo sprintf(' %s ------------ %s ------------------ %d', $record->value('hashtag'), $record->value('mention'),$record->value('cnt'));
	    echo "<br>{";
	    echo "<br>";

	    foreach ($record->value('list') as $key => $value) {
	    	echo $value.", ";
	    }
	    echo "<br>";
	    echo "}";
	    echo "<br>";
	    echo "<br>";

}
?>


--------------------------------------------------------------Que9---------------------------------------------------------


<?php

require_once 'vendor/autoload.php';

use GraphAware\Neo4j\Client\ClientBuilder;

$client = ClientBuilder::create()
    ->addConnection('bolt', 'bolt://neo4j:password@localhost:7687')
    ->build();

$query9 = "match (u:User)<-[:Mentions]-(t:Tweet)-[:Has_hashtag]->(h:Hashtag) where u.author_screen_name="."'narendramodi'"." and t.type="."'Tweet'"." return u.author_screen_name as mention, h.hashtag as hashtag, collect(t.tid) as list order by size(list) desc limit 3;
";

$result = $client->run($query9);

foreach ($result->getRecords() as $record) {
	    echo sprintf(' %s ------------ %s', $record->value('mention'), $record->value('hashtag'));
	    echo "<br>{";
	    echo "<br>";

	    foreach ($record->value('list') as $key => $value) {
	    	echo $value.", ";
	    }
	    echo "<br>";
	    echo "}";
	    echo "<br>";
	    echo "<br>";

}
?>