import sys # used to get argv
import cgi # used to parse Mutlipart FormData 
import os
# web server parts
from http.server import HTTPServer, BaseHTTPRequestHandler
# used to parse the URL and extract form data for GET requests
from urllib.parse import urlparse, parse_qsl
import hclib
import sqlite3
GAME_TIME = 300
DATABASE = 'hostageChess.db'

class DBHandler:
    def __init__(self, db_name='hostageChess.db'):
        self.db_name = db_name

    def connect(self):
        return sqlite3.connect(self.db_name)

    def initializeDB(self):
        """Create tables if they don't exist."""
        conn = self.connect()
        cursor = conn.cursor()

        # Create 'games' table
        cursor.execute("""
        CREATE TABLE IF NOT EXISTS games (
            GAME_NO INTEGER PRIMARY KEY AUTOINCREMENT,
            WHITE_HANDLE TEXT NOT NULL,
            BLACK_HANDLE TEXT,
            RESULT TEXT
        )
        """)

        # Create 'boards' table
        cursor.execute("""
        CREATE TABLE IF NOT EXISTS boards (
            GAME_NO INTEGER NOT NULL,
            TURN_NO INTEGER NOT NULL,
            TURN TEXT NOT NULL,
            BOARD TEXT NOT NULL,
            REAL_TIME TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            WHITE_TIME INTEGER NOT NULL,
            BLACK_TIME INTEGER NOT NULL,
            FOREIGN KEY(GAME_NO) REFERENCES games(GAME_NO)
        )
        """)

        conn.commit()
        conn.close()

#handler for our web-server - handles both GET and POST requests
class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # parse the URL to get the path and form data
        parsed = urlparse(self.path)

        # Serve index.html
        if parsed.path in ['/index.html', '/']:
            try:
                # Construct the path to the index.html file in the client directory
                file_path = os.path.join('client', 'index.html')
                with open(file_path, 'r') as file:
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
        
        elif parsed.path.startswith('/Playerimg/'):
            try:
                file_path = '.' + parsed.path  # Serve the file from the correct directory
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
        elif parsed.path.startswith('/audio/'):
            try:
                file_path = '.' + parsed.path  # Resolve the file path
                with open(file_path, 'rb') as file:
                    self.send_response(200)
                    self.send_header("Content-type", "audio/mpeg")
                    self.send_header("Content-length", os.path.getsize(file_path))
                    self.end_headers()
                    self.wfile.write(file.read())
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")

        elif parsed.path in ['/start.html']:
            try:
                file_path = os.path.join('client', 'start.html')  # Point to client/start.html
                with open(file_path, 'r') as file:
                    content = file.read()
                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")

        elif parsed.path in ['/upload.html']:
            # Existing code for /upload.html
            try:
                file_path = os.path.join('client', 'upload.html')
                with open(file_path, 'r') as file:
                    content = file.read()
                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")

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

            #Read board from form data or uploaded file
            if 'stringboard.txt' in form:
                if form['stringboard.txt'].filename:
                    # Initial file upload handling
                    uploaded_file = form['stringboard.txt']
                    theBoard = uploaded_file.file.read().decode('utf-8')
                    newBoard = hclib.boardstring(theBoard)
                    fen_string = hclib.fen(newBoard, turn, 'KQkq', '-', wtime, btime)
                else:
                    # Handle subsequent form submissions
                    fen_string = form.getvalue('stringboard.txt')
            else:
                self.send_response(400)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>Error: No board data received</h1></body></html>")
                return

            #Helper function to format time
            def format_time(seconds):
                minutes = seconds // 60
                sec = seconds % 60
                return f"{minutes}:{sec:02}"

            formatted_wtime = format_time(wtime)
            formatted_btime = format_time(btime)
            next_turn = 'b' if turn == 'w' else 'w'

            try:
                # Load the template file
                file_path = os.path.join('client', 'board.html')
                with open(file_path, 'r') as file:
                    content = file.read()
                    # Perform substitutions
                    content = content.replace('{fen_string}', fen_string)
                    content = content.replace('{formatted_wtime}', formatted_wtime)
                    content = content.replace('{formatted_btime}', formatted_btime)
                    content = content.replace('{next_turn}', next_turn)
                    content = content.replace('{wtime}', str(wtime))
                    content = content.replace('{btime}', str(btime))

                    # Serve the substituted content
                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")

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
                    fen_string = hclib.fen(board, 'w', 'KQkq', '-', 0, 1)  # Adjust values if needed

                    try:
                        # Load the `display.html` template
                        file_path = os.path.join('client', 'display.html')
                        with open(file_path, 'r') as file:
                            content = file.read()

                            # Replace the placeholder with the generated FEN string
                            content = content.replace('{fen_string}', fen_string)

                            # Serve the dynamically generated HTML
                            self.send_response(200)
                            self.send_header("Content-type", "text/html")
                            self.send_header("Content-length", len(content))
                            self.end_headers()
                            self.wfile.write(bytes(content, "utf-8"))
                    except FileNotFoundError:
                        # Handle missing `display.html`
                        self.send_response(404)
                        self.end_headers()
                        self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")
                else:
                    # Handle case where file has no content
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write(b"<html><body><h1>Error: No file uploaded</h1></body></html>")
            else:
                # Handle invalid form input
                self.send_response(400)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>Error: Invalid form input</h1></body></html>")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python server.py <port>")
        sys.exit(1)

    db_handler = DBHandler()
    db_handler.initializeDB()

    # Initialize and start server
    httpd = HTTPServer( ( '0.0.0.0', int(sys.argv[1]) ), MyHandler )
    print( "Server listing in port:0.0.0.0", int(sys.argv[1]) )
    httpd.serve_forever()