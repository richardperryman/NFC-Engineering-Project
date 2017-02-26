<?php
session_start();

// Experimenting with preparing hmac stuff here
global $data;
$algo = "sha256";
?>

<html>
<head>
	<title>SBACS Management</title>
	<link rel="stylesheet" type="text/css" href="style.css">
	
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js">
        $(document).ready(function(){
            // Todo: add stuff to fade in "auths" on click Manage Authenticators, etc.
        });
	</script>
</head>
<body id="body-color">
	Eyyy <?php echo $_SESSION['hmac-user'] ?> <br>
	<nav>
	  Manage Authenticators | Manage Identities | Manage Locks
	</nav>
	<div id="auths">
	Todo: put some info here
	</div>
	<div id="ids">
	Todo: put some other info here
	</div>
	<div id="locks">
	Todo: put the best info here
	</div>
</body>
</html>
