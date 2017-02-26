<?php
session_start();
?>

<html>
<head>
	<title>SBACS Login</title>
	<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body id="body-color">
	<div id="Sign-In">
		<fieldset style="width:30%">
			<legend>Sensor-Based Access Control System Admin Login</legend>
			<form method="POST" action="login.php"> 
			<?php if (isset($_SESSION['msg'])) { echo $_SESSION['msg']; echo "<br>"; } ?>
			User <br><input type="text" name="user" size="40"><br> 
			Password <br><input type="password" name="pass" size="40"><br>
			<input id="button" type="submit" name="submit" value="Log-In">
			</form>
		</fieldset>
	</div> 
</body>
</html> 
