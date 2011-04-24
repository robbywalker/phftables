<?php

$t = $_GET['t'] ? $_GET['t'] : 3;
$v = $_GET['v'] ? $_GET['v'] : 3;

?>

<html>
<head>
	<title>Table for k-<?php echo "$v-$t"; ?></title>
	<link rel="stylesheet" href="style.css" type="text/css"/>
</head>
<body>

<h1 style="margin-bottom: 3px">Table for PHFN(k,<?php echo "$v,$t"; ?>)</h1>
<div style="margin-left: 35px; font-size: 14px">- <a href="index.php">Home</a></div>
<div style="margin-left: 35px; font-size: 12px">- <div style="width:20px; text-align:center; display: inline">v:</div> <a href="table.php?t=<?php echo $t; ?>&v=<?php echo $v - 1; ?>">-</a> <a href="table.php?t=<?php echo $t; ?>&v=<?php echo $v + 1; ?>">+</a></div>
<div style="margin-left: 35px; font-size: 12px">- <div style="width:20px; text-align:center; display: inline">t:</div> <a href="table.php?v=<?php echo $v; ?>&t=<?php echo $t - 1; ?>">-</a> <a href="table.php?v=<?php echo $v; ?>&t=<?php echo $t + 1; ?>">+</a></div><br/>

<?php

require_once( 'db.php' );
require_once( 'article_support.php' );

// Performing SQL query
$sql = "SELECT p.N,p.k,p.phf_id,p.type,p.source_id,c.name FROM phf AS p LEFT JOIN construction AS c ON p.source_id=c.construction_id WHERE p.t=$t AND p.v=$v ORDER BY k";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());

// Printing results in HTML
echo "<table border=\"0\" cellpadding=\"2\"><tr><td align=\"left\" valign=\"top\">";
echo "<table border=\"1\" cellpadding=\"4\">\n";
echo "<tr><td><b>k</b></td><td><b>N</b></td><td><b>Source</b></td></tr>\n";
while ($row = mysql_fetch_array($result, MYSQL_NUM)) {
   echo "<tr>";
	echo "<td><a href=\"view_array.php?id=" . $row[2] . "\">" . $row[1] . "</a></td><td>" . $row[0] . "</td>";
	echo "<td>";

	if ( $row[3] == 'G' ) {
		print_article_abbr( $row[4] );
	} else {
		echo "<a href=\"construction.php?id=" . $row[4] . "\">" . $row[5] . "</a>";
	}

	echo "</td>";

   echo "</tr>\n";
}
echo "</table>\n";
echo "</td><td>&nbsp;</td><td align=\"center\" valign=\"top\"><b>Graph:</b><br/><br/><img src=\"graph.php?t=$t&v=$v\"/></td></tr></table>";

// Free resultset
mysql_free_result($result);

// Closing connection
mysql_close($dbh);

?>

</body>
</html>
