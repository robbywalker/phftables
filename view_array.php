<?php

require_once( 'db.php' );
require_once( 'article_support.php' );

$id = $_GET['id'];

$sql = "SELECT N,k,v,t,type,source_id FROM phf WHERE phf_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );

list ($N, $k, $v, $t, $type, $source_id) = $row;

?>

<html>
<head>
	<title>PHF(<?php echo "$N; $k,$v,$t";?>)</title>
	<link rel="stylesheet" href="style.css" type="text/css"/>
</head>
<body>

<h1 style="margin-bottom: 3px">PHF(<?php echo "$N; $k,$v,$t";?>)</h1>
<div style="margin-left: 15px; font-size: 14px; margin-top: 0px">- Table for <a href="table.php?v=<?php echo $v;?>&t=<?php echo $t; ?>">PHFN(<?php echo "k,$v,$t"; ?>)<a></div><br/>
<hr><br/>
<?php

$words = array( 'G' => 'Supplied', 'C' => 'Constructed', 'D' => 'Directly Constructed' );

echo "This array is <b>" . $words[$type] . "</b>.<br/>";

# score
echo "<h2>Score</h2>";
$sql = "SELECT COUNT(DISTINCT p.phf_id) FROM phf AS p, phf_ingredient AS i WHERE p.phf_id = i.phf_id AND i.type = 'I' AND i.ingredient_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );
$direct = $row ? $row[0] : 0;

$sql = "SELECT COUNT(DISTINCT p.phf_id) FROM phf AS p, phf_dependency AS d WHERE p.phf_id = d.descendant_id AND d.ancestor_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );
$indirect = $row ? $row[0] : 0;
$indirect -= $direct;

$direct += 1;

$score = $direct + $indirect;
echo "<span class=\"score\">$score</span> = <span class=\"subscore\">$direct</span> direct + <span class=\"subscore\">$indirect</span> indirect.<br/><br/>\n";

# details
if ( $type == 'C' ) {
	echo '<h3>Construction</h3>';
	$sql = "SELECT name FROM construction WHERE construction_id = $source_id";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
	$row = mysql_fetch_array( $result, MYSQL_NUM );

	echo "This PHF was constructed using '<a href=\"construction.php?id=$source_id\">$row[0]</a>'.<br/>";

	echo '<h3>Ingredients</h3>';

	$sql = "SELECT type,ingredient_id,N,k,v,t FROM phf_ingredient WHERE phf_id = $id ORDER BY ingredient_number";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());

	echo "<ul>\n";
	while (  $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
		echo "<li>";
		if ( $row[0] == 'I' ) {
			echo "<a href=\"view_array.php?id=" . $row[1] . "\">";
		}

		echo "PHF(" . $row[2] . '; ' . $row[3] . ',' . $row[4] . ',' . $row[5] . ')';

		if ( $row[0] == 'I' ) {
			echo "</a>";
		}
	}
	echo "</ul>";
} else if ( $type == 'G' ) {
	echo '<h3>Source</h3>This PHF appeared in:<br/> &nbsp;&nbsp;&nbsp; ';
	print_article_reference( $source_id );
	echo '<br/>';
} else {
	echo '<h3>Construction</h3>';
	$sql = "SELECT name FROM construction WHERE construction_id = $source_id";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
	$row = mysql_fetch_array( $result, MYSQL_NUM );

	echo "This PHF was constructed using '<a href=\"construction.php?id=$source_id\">$row[0]</a>'.<br/>";
}

if ( $type == 'C' || $type == 'D' ) {
	$parameters = array();
	$sql = "SELECT parameter_name, value FROM phf_parameter WHERE phf_id = $id";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
	while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
		$parameters[ $row[0] ] = $row[1];
	}

	if ( count( $parameters ) ) {
		echo "<h3>Parameters</h3>\n";
		echo "<ul>\n";
		foreach ( $parameters as $name => $value ) {
			echo "<li><b>$name</b> - $value</li>\n";
		}
		echo "</ul>\n";
	}
}

echo '<h3>Child Arrays</h3>';
$sql = "SELECT DISTINCT p.phf_id,p.N,p.k,p.v,p.t FROM phf AS p, phf_ingredient AS i WHERE i.type='I' AND i.ingredient_id=$id AND i.phf_id=p.phf_id ORDER BY p.t, p.v, p.k";

$count = 0;

$result = mysql_query($sql) or die('Query failed: ' . mysql_error());

echo "<ul>\n";
while (  $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
	echo "<li>";
	echo "<a href=\"view_array.php?id=" . $row[0] . "\">";

	echo "PHF(" . $row[1] . '; ' . $row[2] . ',' . $row[3] . ',' . $row[4] . ')';

	echo "</a>";

	$count++;
}
echo "</ul>";

if ( $count == 0 ) {
	echo 'None.';
}
