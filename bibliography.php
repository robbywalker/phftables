<?php

require_once( 'db.php' );
require_once( 'article_support.php' );

?>

<html>
<head>
	<title>phftables.com: Bibliography</title>
	<link rel="stylesheet" href="style.css" type="text/css"/>
</head>
<body>

<h1 style="margin-bottom: 3px">Bibliography</h1>
<div style="margin-left: 35px; font-size: 14px">- <a href="index.php">Home</a></div><br/><br/>

<?php

$sql = "SELECT article_id FROM article";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$articles = array();
while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
	ob_start();
	$articles[ get_article_sort( $row[0] ) ] = $row[0];
}

ksort( $articles );

$i = 1;
foreach ( $articles as $article ) {
	print "[$i] ";
	print_article_reference( $article );
	print "<br/><br/>";
	$i++;
}

?>
</body></html>


