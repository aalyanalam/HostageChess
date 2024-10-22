import sys # used to get argv
import cgi # used to parse Mutlipart FormData 
import os
# web server parts
from http.server import HTTPServer, BaseHTTPRequestHandler
# used to parse the URL and extract form data for GET requests
from urllib.parse import urlparse, parse_qsl
import hclib

# handler for our web-server - handles both GET and POST requests
class MyHandler( BaseHTTPRequestHandler ):
    def do_GET(self):
        # handle get request
        # parse the URL to get the path and form data
        parsed  = urlparse( self.path )

        if parsed.path in ['/start.html']:

            # retreive the HTML file
            content = """
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Chessboard</title>
                <link rel="stylesheet"
                href="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.css"
                integrity="sha384-q94+BZtLrkL1/ohfjR8c6L+A6qzNH9R2hBLwyoAfu3i/WCvQjzL2RQJ3uNHDISdU"
                crossorigin="anonymous">
            </head>
            <body>
                
                <div id="board" style="width: 400px;"></div>

                <!-- jQuery library (for chessboard.js) -->
                <script src="https://code.jquery.com/jquery-3.5.1.min.js"
                    integrity="sha384-ZvpUoO/+PpLXR1lu4jmpXWu80pZlYUAfxl5NsBMWOEPSjUn/6Z/hRTt8+pR6L4N2"
                    crossorigin="anonymous"></script>

                <!-- chessboard.js library -->
                <script src="https://unpkg.com/@chrisoakman/chessboardjs@1.0.0/dist/chessboard-1.0.0.min.js"
                    integrity="sha384-8Vi8VHwn3vjQ9eUHUxex3JSN/NFqUg3QbPyX8kWyb93+8AC/pPWTzj+nHtbC5bxD"
                    crossorigin="anonymous"></script> 

                <!-- Initialize the chessboard with custom piece images -->
                <script>
                    var board = Chessboard('board', {
                        position: 'start',
                        pieceTheme: 'img/chesspieces/wikipedia/{piece}.png' 
                    });
                </script>
            </body>
            </html>
            """

            # generate the headers
            self.send_response( 200 ) # OK
            self.send_header( "Content-type", "text/html" )
            self.send_header( "Content-length", len( content ) )
            self.end_headers()

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) )

        elif parsed.path.startswith('/img/'):
            try:
                file_path = '.' + self.path  # Add directory path
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


        elif parsed.path in ['/upload.html']:
            # retreive the HTML file
            content = """
            <!DOCTYPE html>
            <html>
            <head>
                <title> Shoot! </title>
            </head>
            <body>
                <form action="/display.html" method="post" enctype="multipart/form-data">
                    <input type="file" id="board" name="stringboard.txt" required>A
                    <input type="submit" value="Submit">
                </form>
            </body>
            </html>
            """
            # generate the headers
            self.send_response( 200 ) # OK
            self.send_header( "Content-type", "text/html" )
            self.send_header( "Content-length", len( content ) )
            self.end_headers()

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) )


    def do_POST(self):
        # handle post request
        parsed = urlparse(self.path)

        if parsed.path in ['/display.html']:
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