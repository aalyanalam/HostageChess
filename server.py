import sys # used to get argv
import cgi # used to parse Mutlipart FormData 
import os
# web server parts
from http.server import HTTPServer, BaseHTTPRequestHandler
# used to parse the URL and extract form data for GET requests
from urllib.parse import urlparse, parse_qsl
import hclib

# handler for our web-server - handles both GET and POST requests
class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # parse the URL to get the path and form data
        parsed = urlparse(self.path)

        # Serve index.html
        if parsed.path in ['/index.html', '/']:
            try:
                with open("index.html", 'r') as file:
                    content = file.read()
                    self.send_response(200)  # OK
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")
        
        # Serve CSS files (added functionality)
        elif parsed.path.startswith('/css/'):
            try:
                file_path = '.' + self.path
                with open(file_path, 'r') as file:
                    content = file.read()
                    self.send_response(200)
                    self.send_header("Content-type", "text/css")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")
        
        # Serve JS files (added functionality)
        elif parsed.path.startswith('/js/'):
            try:
                file_path = '.' + self.path
                with open(file_path, 'r') as file:
                    content = file.read()
                    self.send_response(200)
                    self.send_header("Content-type", "application/javascript")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")
        
        # Serve images from img/chesspieces/ (added functionality)
        elif parsed.path.startswith('/img/'):
            try:
                file_path = '.' + self.path
                with open(file_path, 'rb') as file:
                    self.send_response(200)
                    self.send_header("Content-type", "image/png")
                    self.send_header("Content-length", os.path.getsize(file_path))
                    self.end_headers()
                    self.wfile.write(file.read())
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")

        # Keep existing functionality for start.html and upload.html unchanged
        elif parsed.path in ['/start.html']:
            # Existing code for /start.html
            content = """
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Chessboard</title>
                <link rel="stylesheet" href="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.css">
            </head>
            <body>
                <div id="board" style="width: 400px;"></div>
                <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
                <script src="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.js"></script>
                <script>
                    var board = Chessboard('board', {
                        position: 'start',
                        pieceTheme: 'img/chesspieces/wikipedia/{piece}.png' 
                    });
                </script>
            </body>
            </html>
            """
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

        elif parsed.path in ['/upload.html']:
            # Existing code for /upload.html
            content = """
            <!DOCTYPE html>
            <html>
            <head>
                <title> Upload </title>
            </head>
            <body>
                <form action="/board.html" method="post" enctype="multipart/form-data">
                    <input type="file" id="board" name="stringboard.txt" required><br><br>

                    <!-- turn -->
                    <label for="turn">Turn (w or b):</label><br>
                    <input type="text" id="turn" name="turn" maxlength="1" pattern="[wb]" placeholder="w" required><br><br>

                    <!-- wtime -->
                    <label for="wtime">White Time (seconds):</label><br>
                    <input type="number" id="wtime" name="wtime" min="0" placeholder="300" required><br><br>

                    <!-- btime -->
                    <label for="btime">Black Time (seconds):</label><br>
                    <input type="number" id="btime" name="btime" min="0" placeholder="300" required><br><br>

                    <!-- submit button -->
                    <input type="submit" value="Submit">
                </form>
            </body>
            </html>
            """
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))

    def do_POST(self):
        # handle post request
        parsed = urlparse(self.path)

        if parsed.path in ['/board.html']:
            form = cgi.FieldStorage(fp=self.rfile,
                                headers=self.headers,
                                environ={'REQUEST_METHOD': 'POST',
                                         'CONTENT_TYPE': self.headers['Content-Type']})

            turn = form.getvalue('turn', 'w')
            try:
                wtime = int(form.getvalue('wtime', '0'))
                btime = int(form.getvalue('btime', '0'))
            except ValueError:
                self.send_response(400)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>Error: Invalid time values</h1></body></html>")
                return

            if 'stringboard.txt' in form:
                uploaded_file = form['stringboard.txt']
                if uploaded_file.file:
                    stringboard_data = uploaded_file.file.read().decode('utf-8')
                    board = hclib.boardstring(stringboard_data)
                    fen_string = hclib.fen(board, turn, 'KQkq', '-', wtime, btime)

                    def format_time(seconds):
                        minutes = seconds // 60
                        sec = seconds % 60
                        return f"{minutes}:{sec:02}"

                    formatted_wtime = format_time(wtime)
                    formatted_btime = format_time(btime)
                    next_turn = 'b' if turn == 'w' else 'w'

                    # Generate the HTML with JavaScript for the countdown timer
                    nice_html = f"""
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <title>Chessboard with Timers</title>
                        <link rel="stylesheet" href="/css/chessboard-1.0.0.css">
                        <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
                        <script src="/js/chessboard-1.0.0.js"></script>
                    </head>
                    <body>
                        <h1>Chessboard</h1>
                        <div id="myBoard" style="width: 400px;"></div>
                        <script>
                            var board = Chessboard('myBoard', {{
                                position: '{fen_string}',
                                draggable: true
                            }});
                        </script>

                        <!-- Timer display and "Done" button -->
                        <div style="margin-top: 20px; display: flex; justify-content: flex-start; align-items: center; gap: 10px; width: 400px;">
                            <span id="white-timer">White: {formatted_wtime}</span>

                            <!-- Form for submitting the move -->
                            <form id="doneForm" action="/board.html" method="post" style="display: inline;">
                                <input type="hidden" name="turn" value="{next_turn}">
                                <input type="hidden" id="wtime" name="wtime" value="{wtime}">
                                <input type="hidden" id="btime" name="btime" value="{btime}">
                                <input type="hidden" name="stringboard.txt" value="{stringboard_data}">
                                <button type="submit" style="margin: 0 10px;">Done</button>
                            </form>

                            <span id="black-timer">{formatted_btime} :Black</span>
                        </div>

                        <!-- JavaScript Countdown Timer -->
                        <script>
                            var turn = '{turn}';
                            var wtime = {wtime};
                            var btime = {btime};
                            var whiteTimerEl = document.getElementById('white-timer');
                            var blackTimerEl = document.getElementById('black-timer');
                            var wtimeInput = document.getElementById('wtime');
                            var btimeInput = document.getElementById('btime');

                            function formatTime(seconds) {{
                                var minutes = Math.floor(seconds / 60);
                                var sec = seconds % 60;
                                return minutes + ':' + (sec < 10 ? '0' : '') + sec;
                            }}

                            function updateTimers() {{
                                if (turn === 'w' && wtime > 0) {{
                                    wtime--;
                                    whiteTimerEl.textContent = 'White: ' + formatTime(wtime);
                                    wtimeInput.value = wtime;
                                }} else if (turn === 'b' && btime > 0) {{
                                    btime--;
                                    blackTimerEl.textContent = formatTime(btime) + ' :Black';
                                    btimeInput.value = btime;
                                }}
                            }}

                            // Countdown every second
                            setInterval(function() {{
                                updateTimers();
                            }}, 1000);
                        </script>
                    </body>
                    </html>
                    """

                    # Send the HTML response to the client
                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(nice_html))
                    self.end_headers()
                    self.wfile.write(bytes(nice_html, "utf-8"))

        elif parsed.path in ['/display.html']:
            # Parse the multipart form data
            form = cgi.FieldStorage(fp=self.rfile,
                                    headers=self.headers,
                                    environ={'REQUEST_METHOD': 'POST',
                                            'CONTENT_TYPE': self.headers['Content-Type']})

            # Check if the file was uploaded
            if 'stringboard.txt' in form:
                uploaded_file = form['stringboard.txt']

                # Check if file content is available
                if uploaded_file.file:
                    # Read the content of the uploaded file
                    stringboard_data = uploaded_file.file.read().decode('utf-8')

                    # Convert the stringboard into an exboard_t structure using your C library
                    board = hclib.boardstring(stringboard_data)

                    # Use the fen method to create a FEN string
                    # Assuming 'w', 'KQkq', '-', 0, 1 are the required values
                    fen_string = hclib.fen(board, 'w', 'KQkq', '-', 0, 1)  # Test values; replace if needed


                    # Generate the HTML page with the chessboard
                    nice_html = f"""
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Chessboard Display</title>
                        <link rel="stylesheet" href="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.css">
                        <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
                        <script src="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.js"></script>
                    </head>
                    <body>
                        <h1>Chessboard Display</h1>
                        <div id="board" style="width: 400px;"></div>

                        <script>
                            var board = Chessboard('board','{fen_string}');
                        </script>
                    </body>
                    </html>
                    """

                    # Send the generated HTML page to the client
                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(nice_html))
                    self.end_headers()

                    # Write the response
                    self.wfile.write(bytes(nice_html, "utf-8"))

                else:
                    # Handle the case where no file was uploaded
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write(b"<html><body><h1>Error: No file uploaded</h1></body></html>")
            else:
                # If the file input is not found in the form
                self.send_response(400)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>Error: Invalid form input</h1></body></html>")

if __name__ == "__main__":
    httpd = HTTPServer( ( 'localhost', int(sys.argv[1]) ), MyHandler )
    print( "Server listing in port:  ", int(sys.argv[1]) )
    httpd.serve_forever()