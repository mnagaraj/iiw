<?php
/**
* Yelp API v2.0 code sample.
*
* This program demonstrates the capability of the Yelp API version 2.0
* by using the Search API to query for businesses by a search term and location,
* and the Business API to query additional information about the top result
* from the search query.
*
* Please refer to http://www.yelp.com/developers/documentation for the API documentation.
*
* This program requires a PHP OAuth2 library, which is included in this branch and can be
* found here:
* http://oauth.googlecode.com/svn/code/php/
*
* Sample usage of the program:
* `php sample.php --term="bars" --location="San Francisco, CA"`
*/
// Enter the path that the oauth library is in relation to the php file
require_once('lib/OAuth.php');
// Set your OAuth credentials here
// These credentials can be obtained from the 'Manage API Access' page in the
// developers documentation (http://www.yelp.com/developers)
$CONSUMER_KEY = 'Gb1E24Qi6-YnI_gMNGNUVw';
$CONSUMER_SECRET = 'j7cMLjogvijx90Qxk7GdfBVhyIk';
$TOKEN = 'ZTwzG8mgpLD02BaxvCuIpxB6rS4fp1pc';
$TOKEN_SECRET = '_bUUX_AJnJy_IdyyT7DOScH0h84';
$API_HOST = 'api.yelp.com';
$DEFAULT_TERM = 'dinner';
$DEFAULT_LOCATION = 'San Francisco, CA';
$SEARCH_LIMIT = 3;
$SEARCH_PATH = '/v2/search/';
$BUSINESS_PATH = '/v2/business/';
/**
* Makes a request to the Yelp API and returns the response
*
* @param $host The domain host of the API
* @param $path The path of the APi after the domain
* @return The JSON response from the request
*/
function request($host, $path) {
$unsigned_url = "https://" . $host . $path;
// Token object built using the OAuth library
$token = new OAuthToken($GLOBALS['TOKEN'], $GLOBALS['TOKEN_SECRET']);
// Consumer object built using the OAuth library
$consumer = new OAuthConsumer($GLOBALS['CONSUMER_KEY'], $GLOBALS['CONSUMER_SECRET']);
// Yelp uses HMAC SHA1 encoding
$signature_method = new OAuthSignatureMethod_HMAC_SHA1();
$oauthrequest = OAuthRequest::from_consumer_and_token(
$consumer,
$token,
'GET',
$unsigned_url
);
// Sign the request
$oauthrequest->sign_request($signature_method, $consumer, $token);
// Get the signed URL
$signed_url = $oauthrequest->to_url();
// Send Yelp API Call
$ch = curl_init($signed_url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HEADER, 0);
$data = curl_exec($ch);
curl_close($ch);
return $data;
}
/**
* Query the Search API by a search term and location
*
* @param $term The search term passed to the API
* @param $location The search location passed to the API
* @return The JSON response from the request
*/
function search($term, $location) {
$url_params = array();
$url_params['term'] = $term ?: $GLOBALS['DEFAULT_TERM'];
$url_params['location'] = $location?: $GLOBALS['DEFAULT_LOCATION'];
$url_params['limit'] = $GLOBALS['SEARCH_LIMIT'];
$search_path = $GLOBALS['SEARCH_PATH'] . "?" . http_build_query($url_params);
return request($GLOBALS['API_HOST'], $search_path);
}
/**
* Query the Business API by business_id
*
* @param $business_id The ID of the business to query
* @return The JSON response from the request
*/
function get_business($business_id) {
$business_path = $GLOBALS['BUSINESS_PATH'] . $business_id;
return request($GLOBALS['API_HOST'], $business_path);
}
/**
* Queries the API by the input values from the user
*
* @param $term The search term to query
* @param $location The location of the business to query
*/
function query_api($term, $location) {
$response = json_decode(search($term, $location));
$business_id = $response->businesses[0]->id;
/*print sprintf(
"%d businesses found, querying business info for the top result \"%s\"\n\n",
count($response->businesses),
$business_id
);*/
$response = get_business($business_id);
//print sprintf("Result for business \"%s\" found:\n", $business_id);
$obj = json_decode($response);
$obj->{'reviews'} = "";

$obj->{'snippet_text'} = "";

$str=$obj->{'url'};
list($one,$two,$three,$four,$five)=explode('/',$str);


$end= strtolower($term);
$five_deals = str_replace(' ', '-', $end);
//echo $five_deals;

//list($one,$two,$three,$four,$five)=explode('/',$str);
//list($a,$b,$c,$d)=explode("-",$five);

//$five_deals=$a."-".$b;
$value = "http://www.yelp.com/biz_photos/".$five."?tab=food";
//echo $value;
$value_deals = "http://www.yipit.com/business/".$five_deals."/?from=search";
//$value_deals = "http://www.yipit.com/business/yard-house/?from=search";
//echo $value_deals;
$item="scrapy crawl dinedeals -a restaurant=".$value." -a coupon=".$value_deals;
//$item=escapeshellcmd("/usr/bin/python /var/www/html/myspider/myspider/iiw/try.py");
$o=array();
$o=exec($item,$arr1,$ret);
/*$array=explode(",",$o);
$temp_store=array();
for($i=0;$i<sizeof($array);$i++)
{
$val=explode("'",$array[$i]);
if(isset($val[1]))
{
array_push($temp_store,"http:".$val[1]);
}
}

$obj->{'images_food'}=$temp_store;*/

$output=json_decode($o);
//echo $output->{'yipit'};
$json_array = array();
$obj->{'information'}=$output;
$json_array['is_claimed'] = $obj->{'is_claimed'};
$json_array['rating'] = $obj->{'rating'};
$json_array['rating_img_url']=$obj->{'rating_img_url'};
$json_array['name'] =  $obj->{'name'};
$json_array['url'] = $obj->{'url'};
$json_array['rating_img_url_small'] = $obj->{'rating_img_url_small'};
for($k=0;$k<count($obj->{'categories'});$k++)
{
$json_array['categories'][$k]=$obj->{'categories'}[$k];
}
$json_array['display_phone']=$obj->{'display_phone'};
$json_array['id']=$obj->{'id'};
$json_array['is_closed']=$obj->{'is_closed'};
$json_array['display_phone']=$obj->{'display_phone'};
$json_array['location']['city']=$obj->{'location'}->{'city'};
$json_array['location']['display_address']=$obj->{'location'}->{'display_address'};
$json_array['location']['coordinate']['latitude']=$obj->{'location'}->{'coordinate'}->{'latitude'};
$json_array['location']['coordinate']['longitude']=$obj->{'location'}->{'coordinate'}->{'longitude'};
$json_array['numberofdeals']=$obj->{'information'}->{'numberofdeals'};
$json_array['image_icon']=$obj->{'image_url'};
for($k=0;$k<(int)$obj->{'information'}->{'numberofdeals'};$k++)
{
	$json_array['deals']['yipit'][$k]=$obj->{'information'}->{'deals'}->{'yipit'.$k};
}
$json_array['image_urls']=$obj->{'information'}->{'image_urls'}[0];
//echo count($json_array['image_urls']);
echo json_encode($json_array);

}
/**
* User input is handled here
*/
$longopts = array(
"term::",
"location::",
);
$options = getopt("", $longopts);

$restaurant='';
$loc='';
if(isset($_GET['names']))
{
	$restaurant = $_GET['names'];
	//echo $restaurant;
}

if (isset($_GET['loc']))
{
	$loc=$_GET['loc'];
	//echo $loc;
  	//$term = $restaurant;
  	//echo $restaurant;
  	//echo $location;
}
	

//$term = '';
//$location = 'Los Angeles,CA';
$term=$restaurant;
$location = $loc;
//$term="Yard House";
//$location="Los Angeles,CA";
query_api($term, $location);
?>
