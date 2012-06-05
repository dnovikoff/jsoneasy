<?php

function startsWith($haystack, $needle)
{
    $length = strlen($needle);
    return (substr($haystack, 0, $length) === $needle);
}


function endsWith($haystack, $needle) {
    $length = strlen($needle);
    if ($length == 0) {
        return true;
    }

    $start  = $length * -1; //negative
    return (substr($haystack, $start) === $needle);
}
$arr = array();

$dir = 'data/';

if ($handle = opendir($dir)) {
    while (false !== ($entry = readdir($handle))) {
        if ( !endsWith($entry, ".json") ) continue;
	$arr[] = $entry;
    }
    closedir($handle);
}

sort($arr);

foreach($arr as $entry) {
	$positive = startsWith($entry, "pass");
	$content = file_get_contents($dir.$entry);
	$content = addslashes($content);
	$content = str_replace("\n", "\\n", $content);
	$n = $positive?"":"!";
        echo "\tBOOST_CHECK( /* $entry */ $n parse(\"".($content)."\") );\n";
}
?>
