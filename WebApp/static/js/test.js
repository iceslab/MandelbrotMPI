$(document).ready( function() {
	setInterval(function () {
		$.get("/test_js_func" , function(dane){
			console.log(dane);
			for (var key in dane){
				console.log(key, ": ", dane[key]);
				$('#'+key).text(dane[key]);
			}
    	});
    },1000);
});
