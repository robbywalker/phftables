<html>
<head>
	<title>phftables.com : Array Scores</title>
	<link rel="stylesheet" href="style.css" type="text/css"/>
</head>
<body>

<h1 style="margin-bottom: 3px">Array Scores: Top 200</h1>
<div style="margin-left: 35px; font-size: 14px">- <a href="index.php">Home</a></div>


<?php

require_once( 'db.php' );

$sql = "SELECT p.phf_id,p.N,p.k,p.v,p.t,(COUNT(d.ancestor_id)+1) AS score FROM phf AS p LEFT JOIN phf_dependency AS d ON p.phf_id = d.ancestor_id GROUP BY p.phf_id ORDER BY score DESC,t,v,k LIMIT 200";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());

echo "<table border=\"1\" cellpadding=\"4\">";
while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
	list($id,$N,$k,$v,$t,$score) = $row;

	echo "<tr><td><a href=\"view_array.php?id=$id\">PHF($N; $k,$v,$t)</a></td><td>$score</td></tr>";
}

mysql_free_result($result);

?>

</body></html>
