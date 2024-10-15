from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('mars_rover_control.html')

@app.route('/cam')
def index_cam():
    return render_template('mars_rover_control_cam.html')

if __name__ == '__main__':
    app.run(host="192.168.0.69", port=5000, debug=True)