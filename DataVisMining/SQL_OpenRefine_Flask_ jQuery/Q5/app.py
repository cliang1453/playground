from flask import Flask, render_template, request, json
app = Flask(__name__)

@app.route("/")
def hello():
    return render_template('hello.html')

@app.route('/signup')
def signUp():
    return render_template('signup.html')


def check_validity(password):
	error_indicator = []
	
	if len(password) < 8:
		error_indicator.append(1)
	
	if not any(x.isupper() for x in password):
		error_indicator.append(2)

	if not any(x.isdigit() for x in password):
		error_indicator.append(3)

	return error_indicator

@app.route('/signupuser', methods=['POST'])
def signUpUser():
    user =  request.form['username'];
    password = request.form['password'];
    error_indicator = check_validity(password);

    if len(error_indicator)==0:
    	return json.dumps({'status':'OK','user':user,'pass':password});
    else:
    	return json.dumps({'status':'BAD','user':user,'pass':error_indicator});

if __name__ == "__main__":
    app.run()