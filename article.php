<?php

require_once( 'db.php' );
require_once( 'article_support.php' );

$id = $_GET['id'];

$sql = "SELECT title FROM article WHERE article_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );

list ($title) = $row;

?>

<html>
<head>
	<title>phftables.com: Article <?php echo $title; ?></title>
	<link rel="stylesheet" href="style.css" type="text/css"/>
</head>
<body>

<?php

echo "<h2>$title</h2>\n";

print_article_reference( $id );

# score
$sql = "SELECT c.construction_id, c.name FROM construction AS c WHERE c.source_article_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$constrs = array();
while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
	$constrs[ $row[0] ] = $row[1];
}
$constr_list = implode( ', ', array_keys( $constrs ) );

echo "</br><h2>Score</h2>\n";
$condition = '';
if ( $constr_list ) {
	$condition .= "((p.type='C' OR p.type='D') AND p.source_id IN ($constr_list)) OR ";
}
$condition .= "(p.type='G' AND p.source_id=$id)";

$sql = "SELECT COUNT(*) FROM phf AS p WHERE $condition";
$result2 = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row2 = mysql_fetch_array( $result2, MYSQL_NUM );
$direct = $row2[0];

$p2condition = str_replace('p.', 'p2.',$condition);
$sql = "SELECT COUNT(DISTINCT p2.phf_id) FROM phf AS p, phf_dependency AS d, phf AS p2 WHERE p2.phf_id = d.descendant_id AND p.phf_id = d.ancestor_id AND ($condition) AND !( $p2condition )";
$result2 = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row2 = mysql_fetch_array( $result2, MYSQL_NUM );
$indirect = $row2[0];

$score = $direct + $indirect;
echo "<span class=\"score\">$score</span> = <span class=\"subscore\">$direct</span> direct + <span class=\"subscore\">$indirect</span> indirect.<br/><br/>\n";

# constructions
if ( count( $constrs ) ) {
	echo "<br/><h3>Constructions</h3>\n<ul>\n";

	foreach ( $constrs as $cId => $name ) {
		echo "<li>";
		echo "<a href=\"construction.php?id=$cId\">$name</a>";
		echo "</li>";
	}

	echo "</ul>";
}

# arrays
$sql = "SELECT p.phf_id, p.N, p.k, p.v, p.t FROM phf AS p WHERE p.type='G' AND p.source_id=$id ORDER BY t,v,k";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );

if ( $row ) {
	echo "<br/><h3>PHFs</h3>\n<ul>\n";

	do {
		list( $pId,$N,$k,$v,$t) = $row;
		echo "<li>";
		echo "<a href=\"view_array.php?id=$pId\">PHF($N; $k,$v,$t)</a>";
		echo "</li>";
	} while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) );

	echo "</ul>";
}


?>


