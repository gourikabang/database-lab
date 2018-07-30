<?php
$cluster = Cassandra::cluster()->build();
$keyspace = 'key_150101062';
$session = $cluster->connect($keyspace);

$query = new Cassandra\SimpleStatement("SELECT * FROM key_150101062.que6  where location = 'Delhi' and date_ >= '2018-02-02' and date_ <= '2018-02-10'");
$result = $session->execute($query);

foreach($result as $row){
	echo $row['location'].",";
	echo $row['date_'].",";
	echo $row['tid_count'];
	echo "<br>";
}
?>

