<?php

require_once( 'db.php' );

?><html>
<head>
<title>PHFtables</title>
<link rel="stylesheet" href="style.css"/>
</head>
<body>
<img src="logo.gif"/>
<br/><br/><hr/>

<div style="margin-left: 12px">
 Currently containing <?php
 $result = mysql_query( "SELECT COUNT(*) FROM phf" ) or die('Query failed: ' . mysql_error());
 $row = mysql_fetch_array( $result, MYSQL_NUM );
 echo $row[0];
 ?> PHFs.<br/><br/>

 View arrays sorted by <a href="score.php">score</a>.<br/>
</div>
<hr/>

<?php

$arr = array();
$maxv = 0;
$maxt = 0;

$result = mysql_query( "SELECT DISTINCT v,t FROM phf ORDER BY t,v" ) or die('Query failed: ' . mysql_error());
while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
	list($v,$t) = $row;

	if ( ! $arr[ $v ] ) {
		$arr[$v] = array();
	}

	if ( $v > $maxv ) {
		$maxv = $v;
	}

	if ( $t > $maxt ) {
		$maxt = $t;
	}

	$arr[$v][$t] = "<a href=\"table.php?v=$v&t=$t\">PHFN(k,$v,$t)</a>\n";
}

echo "<table cellpadding=\"6\">";
for ( $v = 3; $v <= $maxv; $v++ ) {
	echo "<tr>";

	for ( $t = 3; $t <= $maxt; $t++ ) {
		echo "<td align=\"left\">";
		$cell = $arr[$v][$t];
		echo $cell ? $cell : '&nbsp;';
		echo "</td>";
	}

	echo "</tr>";
}
echo "</table>";

?>

</ul>

<hr/>

<table cellpadding="8" border="0">
<tr>
	<td>&nbsp;</td>
	<td><a href="bibliography.php">Bibliography</a></td>
	<td>&nbsp;</td>
	<td><a href="about.php">About</a></td>
</tr>
</body>
</html>
