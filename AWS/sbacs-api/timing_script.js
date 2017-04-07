const crypto = require('crypto');

salt = crypto.randomBytes(255);

function time_pbkdf2(iterations, algorithm) {

	secret = crypto.pbkdf2Sync("", salt, iterations, 255, algorithm);

}

const algorithms = ["sha256"];
const iterations = 100;

for(var algo = 0; algo < algorithms.length; algo++) {
	for(var i = 1e4; i < 1e5; i*=10) {
		for(var salt_len = 1; salt_len <= 256; salt_len*=2){
			console.log(i + " " + salt_len);
			var min = 10;
			var max = 0;
			var start = process.hrtime();
			for (var iters = 0; iters < iterations; iters++) {
				var local_start = process.hrtime();
				time_pbkdf2(i, algorithms[algo]);
				var local_end = process.hrtime(local_start);
				var elapsed = local_end[0] + local_end[1] / 1e9;
				if (elapsed < min) {
					min = elapsed;
				} else if (elapsed > max) {
					max = elapsed;
				}
			}
			var end = process.hrtime(start);
			var avg = (end[0] + end[1] / 1e9) / iterations;
			console.log("min: " + min + ", max: " + max + " average: " + avg);
		}
	}
}