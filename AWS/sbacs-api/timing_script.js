const crypto = require('crypto');

salt = crypto.randomBytes(255);

function time_pbkdf2(iterations, algorithm) {

	secret = crypto.pbkdf2Sync("", salt, iterations, 255, algorithm);

}

const algorithms = ["sha256", "sha1"];

for(var algo = 0; algo < algorithms.length; algo++) {
	for(var i = 1; i <= 1e6; i*=10) {
		var timer_name = "time pbkdf2 with " + i + " iterations using " + algorithms[algo];
		console.time(timer_name);
		time_pbkdf2(i, algorithms[algo]);
		console.timeEnd(timer_name);
	}
}