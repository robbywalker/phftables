<?php

require_once( 'db.php' );
require_once( 'article_support.php' );

$id = $_GET['id'];

$sql = "SELECT name, num_ingredients, source_article_id FROM construction WHERE construction_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );

list ($name, $num_ingredients, $article) = $row;

?>

<html>
<head>
	<title>phftables.com: Construction '<?php echo $name; ?>'</title>
	<link rel="stylesheet" href="style.css" type="text/css"/>
</head>
<body>

<?php

echo "<h1>$name</h1>\n";

print "<b>Appeared in: </b>";
print_article_reference( $article );
print "<br/><br/>";

$type = $num_ingredients ? 'Recursive' : 'Direct';
print "This is a <b>$type</b> construction.<br/><br/>\n";

if ( $num_ingredients ) {
	print "It uses <b>$num_ingredients</b> ingredients.<br/><br/>\n";
}

# score
echo "</br><h2>Score</h2>\n";
$sql = "SELECT COUNT(*) FROM phf AS p WHERE (p.type='C' OR p.type='D') AND p.source_id=$id";
$result2 = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row2 = mysql_fetch_array( $result2, MYSQL_NUM );
$direct = $row2[0];

$sql = "SELECT COUNT(DISTINCT p.phf_id) FROM phf AS p, phf_dependency AS d, phf AS p2 WHERE p.phf_id = d.descendant_id AND p2.phf_id = d.ancestor_id AND ((p2.type='C' OR p2.type='D') AND p2.source_id=$id) AND !((p.type='C' OR p.type='D') AND p.source_id=$id)";
$result2 = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row2 = mysql_fetch_array( $result2, MYSQL_NUM );
$indirect = $row2[0];

$score = $direct + $indirect;
echo "<span class=\"score\">$score</span> = <span class=\"subscore\">$direct</span> direct + <span class=\"subscore\">$indirect</span> indirect.<br/><br/>\n";

# phfs
$sql = "SELECT p.phf_id, p.N, p.k, p.v, p.t FROM phf AS p WHERE (p.type='C' OR p.type='D') AND p.source_id=$id ORDER BY t,v,k";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );

if ( $row ) {
	echo "<br/><h3>PHFs</h3>\n";

	$sql = "SELECT COUNT(*) FROM phf AS p WHERE (p.type='C' OR p.type='D') AND p.source_id=$id";
	print "This construction produces $direct PHFs<br/><br/>\n";

	print "<ul>\n";

	do {
		list( $pId,$N,$k,$v,$t) = $row;
		echo "<li>";
		echo "<a href=\"view_array.php?id=$pId\">PHF($N; $k,$v,$t)</a>";
		echo "</li>";
	} while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) );

	echo "</ul>";
}


?>


