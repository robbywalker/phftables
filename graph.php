<?php

require_once( 'db.php' );

$t = $_GET['t'] ? $_GET['t'] : 3;
$v = $_GET['v'] ? $_GET['v'] : 3;

header('Content-type: image/gif');

$width = 220;
$height = 190;
$border = 20;

$im = imagecreate( $width + $border * 2, $height + $border * 2 );
$bg = imagecolorallocate($im, 240, 240, 240);
$black = imagecolorallocate($im, 20, 20, 100);
$point = imagecolorallocate($im, 0, 0, 0);
$text = imagecolorallocate($im, 100, 20, 20 );

imageline($im,$border,$border,$border,$border + $height,$black);
imageline($im,$border,$border + $height,$border + $width,$border + $height,$black);

$xmin = 0;
$xmax = 0;
$ymin = 0;
$ymax = 0;

$sql = "SELECT p.N,p.k,p.type FROM phf AS p WHERE p.t=$t AND p.v=$v ORDER BY k";
$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
$xpoints = array();
$ypoints = array();
$types = array();
while ($row = mysql_fetch_array($result, MYSQL_NUM)) {
	list( $N, $k, $type ) = $row;
	array_push( $xpoints, log( $k ) );
	array_push( $ypoints, $N );
	array_push( $types, $type );

	if ( ceil( log( $k ) ) > $xmax  ) {
		$xmax = ceil( log( $k ) );
	}

	if ( $k > $ymax )	{
		$ymax = $N;
	}
}

for ( $i = 0; $i < count($xpoints); $i++ ) {
	$xpixel = round( ( $xpoints[$i] * $width ) / $xmax ) + $border;
	$ypixel = ($border + $height) - round( ( $ypoints[$i] * $height ) / ( 1.0 * $ymax ) );
	$type = $types[$i];

	if ( $type == 'C' ) {
		imageline( $im, $xpixel - 2, $ypixel, $xpixel + 2, $ypixel, $point );
		imageline( $im, $xpixel, $ypixel - 2, $xpixel, $ypixel + 2, $point );
	} else if ( $type == 'D' ) {
		imageline( $im, $xpixel - 2, $ypixel - 2, $xpixel + 2, $ypixel + 2, $point );
		imageline( $im, $xpixel + 2, $ypixel - 2, $xpixel - 2, $ypixel + 2, $point );
	} else if ( $type == 'G' ) {
		imagearc( $im, $xpixel, $ypixel, 5, 5, 0, 360, $point );
	}
}

imagestring( $im, 2, $width / 2 + $border - 15, $border + $height + 3, 'log k', $text );
imagestring( $im, 2, $border - 13, $height / 2 + $border - 5, 'N', $text );

imagegif($im);
imagedestroy($im);

?>
