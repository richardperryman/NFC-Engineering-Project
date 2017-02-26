<?php
session_start();
$curl = curl_init();

$url = 'https://sbacs.click/login';

$fields = array('username' => $_POST['user'], 'password' => $_POST['pass']);
print json_encode($fields);

curl_setopt($curl, CURLOPT_URL, $url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
curl_setopt($curl, CURLOPT_FOLLOWLOCATION, true);
curl_setopt($curl, CURLOPT_POST, 1);
curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($fields));
$result = curl_exec($curl);
print curl_error($curl);
print curl_getinfo($curl, CURLINFO_RESPONSE_CODE);

if (curl_getinfo($curl, CURLINFO_RESPONSE_CODE) == 200) {
	unset($_SESSION['msg']);
	print($result);
	print('<br>');
	$info = json_decode($result, true);
	
	$_SESSION['hmac-user'] = $_POST['user'];
	$_SESSION['user_id'] = $info['user'];
	$_SESSION['user_key'] = $info['key']['data'];
	header('Location: management.php');
} else {
	$_SESSION['msg'] = 'Unable to verify login information.';
	header('Location: index.php');
}

?>