<?php

require_once( 'db.php' );
require_once( 'article_support.php' );

$id = $_GET['id'];

$sql = "SELECT name,email,homepage FROM author WHERE author_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$row = mysql_fetch_array( $result, MYSQL_NUM );

list ($name, $email, $homepage) = $row;

?>

<html>
<head>
	<title>phftables.com: Author <?php echo $name; ?></title>
	<link rel="stylesheet" href="style.css" type="text/css"/>
</head>
<body>

<?php

echo "<h1>$name</h1>\n";

if ( $email ) {
	echo "Email: <a href=\"mailto:$email\">$email</a><br/>";
}
if ( $homepage ) {
	echo "Homepage: <a href=\"$homepage\">$homepage</a><br/>";
}

# score
$articles = array();
$sql = "SELECT a.article_id FROM article AS a, article_author AS aa WHERE a.article_id = aa.article_id AND aa.author_id = $id";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error(). ';;; $sql');
while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
	array_push( $articles, $row[0] );
}
if ( $articles ) {
	$article_list = implode( ',', $articles );

	$sql = "SELECT c.construction_id FROM construction AS c WHERE c.source_article_id IN ($article_list)";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error(). ';;; $sql');
	$constrs = array();
	while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
		array_push( $constrs, $row[0] );
	}
	$constr_list = implode( ',', $constrs );

	echo "</br><h2>Score</h2>\n";
	$condition = '';
	if ( $constr_list ) {
		$condition .= "((p.type='C' OR p.type='D') AND p.source_id IN ($constr_list)) OR ";
	}
	$condition .= "(p.type='G' AND p.source_id IN ($article_list))";

	$sql = "SELECT COUNT(*) FROM phf AS p WHERE $condition";
	$result2 = mysql_query($sql) or die('Query failed: ' . mysql_error(). ';;; $sql');
	$row2 = mysql_fetch_array( $result2, MYSQL_NUM );
	$direct = $row2[0];

	$p2condition = str_replace('p.', 'p2.',$condition);
	$sql = "SELECT COUNT(DISTINCT p2.phf_id) FROM phf AS p, phf_dependency AS d, phf AS p2 WHERE p2.phf_id = d.descendant_id AND p.phf_id = d.ancestor_id AND ($condition) AND !( $p2condition )";
	$result2 = mysql_query($sql) or die('Query failed: ' . mysql_error(). ';;; $sql');
	$row2 = mysql_fetch_array( $result2, MYSQL_NUM );
	$indirect = $row2[0];

	$score = $direct + $indirect;
	echo "<span class=\"score\">$score</span> = <span class=\"subscore\">$direct</span> direct + <span class=\"subscore\">$indirect</span> indirect.<br/><br/>\n";
} else {
	echo "<span class=\"score\">0</span>";
}

echo "<br/><h3>Articles</h3>\n<ul>\n";

foreach ( $articles as $article ) {
	echo "<li>";
	print_article_reference( $article );
	echo "</li>";
}

echo "</ul>";

?>


