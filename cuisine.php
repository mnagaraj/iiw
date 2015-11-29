<?php

$food='';
if(isset($_GET['name']))
{
	$food=$_GET['name'];
}
if(isset($_GET['city']))
{
	$city=$_GET['city'];
	$city1 = str_replace(' ', '-', $city);
}

$cuisine = "http://www.groupon.com/browse/".$city1."?category=food-and-drink&category3=".$food;
$cuisine1=urlencode($cuisine);
$item="scrapy crawl getdeals -a cuisine=".$cuisine1;

$o=array();
$o=exec($item,$arr1,$ret);


echo $o;

?>
