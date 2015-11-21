<?php

header('content-type:application/json; charset=utf-8');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST');

ini_set('max_execution_time', 100);
ini_set('memory_limit', '128M');
/*if(isset($_POST['names']))
{
	$restaurant=$_POST['names'];
}*/
$json_array=array();
/*
$restaurant=get the arguement from js.
*/
/*if(isset($_GET['names']))
{
	$data = json_decode(stripslashes($_GET['names']));

  // here i would like use foreach:

  //foreach($data as $d){
   echo $data;
  //}
}*/
$restaurant="Yard House";
//$restaurant=$data;
$item=escapeshellcmd("/usr/bin/python /var/www/html/myspider/myspider/iiw/yelp.py -q '".$restaurant."'");
//$item=escapeshellcmd("/usr/bin/python /var/www/html/myspider/myspider/iiw/try.py");
$arr1=array();

$exec_enabled =
     function_exists('exec') &&
     !in_array('exec', array_map('trim',explode(', ', ini_get('disable_functions')))) &&
              !(strtolower( ini_get( 'safe_mode' ) ) != 'off');

$output=exec($item,$arr1,$ret);
//echo $ret;
//$temp=json_decode(preg_replace('/[\x00-\x1F\x80-\xFF]/','',$output),true);
//$temp=preg_replace("/\r(?!\n)/", '', $output);
//$tp=json_decode($output);
//echo $tp;
//$o=str_replace("'","",$output);
if($ret==0)
{
	//console.log("hello");



$o=str_replace("'","",$output);
$x=str_replace("\\","",$o);	
$y=json_decode($x,false);
//var_dump($y->rating);
$json_array['name'] ="ass";
//= (string)$y->{'name'};
//echo $json_array['name'];

$a1=$y->{'rating'};
//echo $a1;
if($a1!=null)
{
$json_array['rating'] = $a1;
}
$json_array['phone'] = (string)$y->{'display_phone'};

/*$temparray = $y->{'images_food'};
for($i=0;$i< sizeof($temparray);$i++)
{
$json_array['images_urls']['img'.$i]=(string)$temparray[$i]; 
}*/
if($y->{'is_closed'} === FALSE)
{
$json_array['closed'] = (string)"YYY";
}
else
{
$json_array['closed'] = (string)"NNN";
}

//$json_array['categories'] = (string)$y->{'categories'}[0];
//$json_array['location'] = (string)$y->{'location'};
//echo $json_array['phone'];
//echo $json_array;
//$me="hello";
//echo json_encode($json_array);
/*ob_start();
var_dump($json_array);
$result=ob_get_clean();
echo json_encode($result);*/
//$me=$json_array['phone'];

echo json_encode($json_array);
//exec();
}

else
{
	$r="buzzy";
	echo json_encode($ret);
}
?>




