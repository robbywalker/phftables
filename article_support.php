<?php

function print_article_reference($article_id) {
	$sql = "SELECT title, journal, issue, year, pages FROM article WHERE article_id=$article_id";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
	$row = mysql_fetch_array( $result, MYSQL_NUM );

	list( $title, $journal, $issue, $year, $pages ) = $row;

	$sql = "SELECT a.author_id, a.name FROM author AS a, article_author AS aa WHERE aa.author_id = a.author_id AND aa.article_id=$article_id ORDER BY aa.sort_order";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());

	$first = 1;
	while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
		if ( $first ) {
			$first = 0;
		} else {
			echo ", ";
		}
		list( $a_id, $a_name ) = $row;
		echo "<a class=\"abbr\" href=\"author.php?id=$a_id\">$a_name</a>";
	}

	echo ", <a href=\"article.php?id=$article_id\">$title</a>, <i>$journal</i>";
	if ( $issue ) {
		echo ", $issue";
	}
	if ( $year ) {
		if ( $issue ) {
			echo " ($year)";
		} else {
			echo ", $year";
		}
	}
	if ( $pages ) {
		echo ", $pages";
	}
}

function get_article_sort($article_id) {
	$sql = "SELECT title, year FROM article WHERE article_id=$article_id";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
	$row = mysql_fetch_array( $result, MYSQL_NUM );

	list( $title, $year ) = $row;

	$sql = "SELECT a.author_id, a.sortname FROM author AS a, article_author AS aa WHERE aa.author_id = a.author_id AND aa.article_id=$article_id ORDER BY aa.sort_order";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());

	$out = '';

	$first = 1;
	while ( $row = mysql_fetch_array( $result, MYSQL_NUM ) ) {
		if ( $first ) {
			$first = 0;
		} else {
			$out .= ", ";
		}
		list( $a_id, $a_name ) = $row;
		$out .= "$a_name";
	}

   $out .= ", $year, $title";
	return $out;
}

function print_article_abbr($article_id) {
	$sql = "SELECT gen_abbr FROM article WHERE article_id=$article_id";
	$result = mysql_query($sql) or die('Query failed: ' . mysql_error());
	$row = mysql_fetch_array( $result, MYSQL_NUM );

	list( $abbr ) = $row;
	if ( ! $abbr ) {
		$abbr = 'article-' . $article_id;
	}

	echo "<a class=\"abbr\" href=\"article.php?id=$article_id\">$abbr</a>";
}



?>
