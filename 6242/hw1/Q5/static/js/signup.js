var errors = ["", ". Should be at least 8 characters in length, ", 
". Should have at least 1 uppercase character, ", 
". Should have at least 1 number. "];


$(function(){
	$('button').click(function(){
		var user = $('#inputUsername').val();
		var pass = $('#inputPassword').val();

		$.ajax({
			url: '/signupuser',
			data: $('form').serialize(),
			type: 'POST',
			success: function(response) {
				console.log(response);
				res = JSON.parse(response);
				if (res["status"] == "BAD") 
				{
					error_msg = res["pass"].map((x,i)=>((i+1) + errors[x])).join(' ');
				    $("#msg").html(user + ", your password must: " + error_msg + "Please Try Again!");
		    		$('#inputPassword').val(''); 
	            } 
	            else 
	            {
				    $("#msg").html("Congratulations on registering for CSE6242, " + user + "! Redirecting you to the course homepage...");
				    $('#inputUsername').val('');
				    $('#inputPassword').val('');
				    setTimeout(function(){ window.location = 'http://poloclub.gatech.edu/cse6242'; }, 3000);
				}
			},
			error: function(error){
				console.log(error);
			}
		})
	});
});

