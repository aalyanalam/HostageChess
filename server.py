import sys # used to get argv
import cgi # used to parse Mutlipart FormData 
import os
# web server parts
from http.server import HTTPServer, BaseHTTPRequestHandler
# used to parse the URL and extract form data for GET requests
from urllib.parse import urlparse, parse_qsl
import hclib
import sqlite3
import json
from urllib.parse import urlparse, parse_qsl, unquote
from datetime import datetime


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
        #This part took way too long
        elif parsed.path in ['/login.html']:
            try:
                file_path = os.path.join('client', 'login.html')
                with open(file_path, 'r') as file:
                    content = file.read()

                    #Inject GAME_NO 
                    game_no = dict(parse_qsl(parsed.query)).get('game_no', '')
                    content = content.replace("{GAME_NO}", game_no)

                    #Serve HTML
                    self.send_response(200)  # OK
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
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

        elif parsed.path in ['/history.html']:
            try:
                conn = sqlite3.connect(DATABASE)
                cursor = conn.cursor()

                # Fetch all rows from the games table
                cursor.execute("SELECT GAME_NO, WHITE_HANDLE, BLACK_HANDLE, RESULT FROM games")
                games = cursor.fetchall()
                conn.close()

                # Load the history.html file
                file_path = os.path.join('client', 'history.html')
                with open(file_path, 'r') as file:
                    content = file.read()

                # Construct a table with games data
                rows = ""
                for game_no, white_handle, black_handle, result in games:
                    rows += f"<tr onclick=\"window.location.href='/gamelog.html?game_no={game_no}'\">"
                    rows += f"<td>{game_no}</td><td>{white_handle}</td><td>{black_handle}</td><td>{result}</td></tr>"

                # Replace the placeholder in history.html with the table rows
                content = content.replace("{games_table}", rows)

                # Serve the updated HTML content
                self.send_response(200)
                self.send_header("Content-type", "text/html")
                self.send_header("Content-length", len(content))
                self.end_headers()
                self.wfile.write(bytes(content, "utf-8"))
            except Exception as e:
                print(f"Error in /history.html: {e}")
                self.send_response(500)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>500 Internal Server Error</h1></body></html>")
        
        elif parsed.path in ['/gamelog.html']:
            try:
                print("DEBUG: Entering /gamelog.html handler.")
                
                # Parse query parameters
                query_params = dict(parse_qsl(parsed.query))
                game_no = query_params.get('game_no')

                if not game_no:
                    print("DEBUG: Missing game_no parameter.")
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write(b"<html><body><h1>400 Bad Request: Missing game_no parameter</h1></body></html>")
                    return
                
                conn = sqlite3.connect(DATABASE)
                cursor = conn.cursor()

                # Fetch all turns for the specified GAME_NO
                cursor.execute("""
                    SELECT TURN_NO, TURN, BOARD FROM boards WHERE GAME_NO = ? ORDER BY TURN_NO ASC
                """, (game_no,))
                turns = cursor.fetchall()

                # Fetch the game's result
                cursor.execute("""
                    SELECT RESULT FROM games WHERE GAME_NO = ?
                """, (game_no,))
                result = cursor.fetchone()

                conn.close()

                if not turns:
                    print("DEBUG: No turns found for Game No:", game_no)
                    self.send_response(404)
                    self.end_headers()
                    self.wfile.write(b"<html><body><h1>404 Game Not Found</h1></body></html>")
                    return

                # Prepare the game log HTML
                file_path = os.path.join('client', 'gamelog.html')
                with open(file_path, 'r') as file:
                    content = file.read()

                    turn_details = ""
                    for turn_no, turn, board in turns:
                        turn_details += f"""
                            <div class="turn-details">
                                <h3>Turn {turn_no}: {turn}'s Move</h3>
                                <div class="board" data-board="{board}"></div>
                            </div>
                        """

                    game_result = result[0] if result else "No result available."

                    content = content.replace('{game_no}', str(game_no))
                    content = content.replace('{turn_details}', turn_details)
                    content = content.replace('{game_result}', game_result)

                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))

            except Exception as e:
                print(f"DEBUG: Error in /gamelog.html: {e}")
                self.send_response(500)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>500 Internal Server Error</h1></body></html>")
        
        elif parsed.path == '/done.js':
            try:
                query_params = dict(parse_qsl(parsed.query))
                game_no = query_params.get('game_no', '')
                turn_no = query_params.get('turn_no', '')

                print(f"Game Number in /done.js: {game_no}, Turn Number: {turn_no}")

                file_path = './done.js'
                with open(file_path, 'r') as file:
                    content = file.read()
                    # Replace placeholders with actual game data
                    content = content.replace('{game_no}', str(game_no))
                    content = content.replace('{turn_no}', str(turn_no))

                    self.send_response(200)
                    self.send_header("Content-type", "application/javascript")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>404 Not Found</h1></body></html>")

        elif parsed.path in ['/player.html']:
            try:
                print("DEBUG: Entering /player.html handler.")

                # Extract query parameters
                query_params = dict(parse_qsl(parsed.query))
                game_no = query_params.get('game_no')
                turn_no = query_params.get('turn_no')

                if not game_no or not turn_no:
                    print("DEBUG: Missing game_no or turn_no parameters.")
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write(b"<html><body><h1>400 Bad Request: Missing parameters</h1></body></html>")
                    return

                turn_no = int(turn_no)
                conn = sqlite3.connect(DATABASE)
                cursor = conn.cursor()

                # Fetch current turn details
                cursor.execute("""
                    SELECT TURN, BOARD, REAL_TIME, WHITE_TIME, BLACK_TIME
                    FROM boards
                    WHERE GAME_NO = ? AND TURN_NO = ?
                """, (game_no, turn_no))
                row = cursor.fetchone()

                if not row:
                    print(f"DEBUG: No board data found for Game No: {game_no}, Turn No: {turn_no}.")
                    self.send_response(404)
                    self.end_headers()
                    self.wfile.write(b"<html><body><h1>404 Turn Not Found</h1></body></html>")
                    conn.close()
                    return

                turn_color, board, real_time, white_time, black_time = row

                # Format timers for display
                def format_time(seconds):
                    minutes = seconds // 60
                    sec = seconds % 60
                    return f"{minutes}:{sec:02}"

                formatted_wtime = format_time(white_time)
                formatted_btime = format_time(black_time)

                print(f"DEBUG: Board: {board}, White Time: {white_time}, Black Time: {black_time}")

                # Serve player.html
                file_path = os.path.join('client', 'player.html')
                with open(file_path, 'r') as file:
                    content = file.read()
                    content = content.replace('{fen_string}', board)
                    content = content.replace('{formatted_wtime}', formatted_wtime)
                    content = content.replace('{formatted_btime}', formatted_btime)
                    content = content.replace('{wtime}', str(white_time))
                    content = content.replace('{btime}', str(black_time))
                    content = content.replace('{game_no}', str(game_no))
                    content = content.replace('{turn_no}', str(turn_no))
                    content = content.replace('{next_turn}', turn_color)

                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))

                conn.close()
            except Exception as e:
                print(f"DEBUG: Error processing /player.html: {e}", flush=True)
                self.send_response(500)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>500 Internal Server Error</h1></body></html>")
        
        elif parsed.path in ['/opponent.html']:
            try:
                print("DEBUG: Entering /opponent.html handler.")
                
                # Parse query parameters
                query_params = dict(parse_qsl(parsed.query))
                game_no = query_params.get('game_no')
                turn_no = query_params.get('turn_no')
                board = query_params.get('board')  # Optional, only present if coming from /player.html

                if not game_no or not turn_no:
                    print("DEBUG: Missing game_no or turn_no parameters.")
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write(b"<html><body><h1>400 Bad Request: Missing parameters</h1></body></html>")
                    return

                turn_no = int(turn_no)
                conn = sqlite3.connect(DATABASE)
                cursor = conn.cursor()
                
                if board:
                    print(f"DEBUG: Adding new row for Game No: {game_no}, Turn No: {turn_no}.")

                    # Check if either King is missing
                    if 'K' not in board or 'k' not in board:
                        winner = "White" if 'k' not in board else "Black"
                        loser = "Black" if winner == "White" else "White"
                        result = f"{winner} wins: Opponent's king was captured."

                        # Insert the new turn into the database for the other player to be redirected
                        next_turn_color = loser
                        cursor.execute("""
                            INSERT INTO boards (GAME_NO, TURN_NO, TURN, BOARD, REAL_TIME, WHITE_TIME, BLACK_TIME)
                            VALUES (?, ?, ?, ?, datetime('now'), ?, ?)
                        """, (game_no, turn_no, next_turn_color, board, 0, 0))  # Timers are set to 0 as the game is over
                        conn.commit()

                        # Update the games table with the result
                        cursor.execute("""
                            UPDATE games SET RESULT = ? WHERE GAME_NO = ?
                        """, (result, game_no))
                        conn.commit()

                        print(f"DEBUG: Game result updated: {result}")

                        # Serve a game-over message
                        result_page_content = f"""
                        <html>
                        <body style="background-color: #728FCE; color: white;">
                            <h1>{result}</h1>
                            <p><a href="/history.html" style="color: lightblue;">View Game History</a></p>
                            <p><a href="/index.html" style="color: lightblue;">Return to Home</a></p>
                        </body>
                        </html>
                        """
                        self.send_response(200)
                        self.send_header("Content-type", "text/html")
                        self.send_header("Content-length", len(result_page_content))
                        self.end_headers()
                        self.wfile.write(bytes(result_page_content, "utf-8"))
                        conn.close()
                        return

                    # Fetch previous turn details
                    cursor.execute("""
                        SELECT TURN, REAL_TIME, WHITE_TIME, BLACK_TIME FROM boards
                        WHERE GAME_NO = ? AND TURN_NO = ?
                    """, (game_no, turn_no - 1))
                    last_turn = cursor.fetchone()

                    if last_turn:
                        last_turn_color, last_real_time, last_white_time, last_black_time = last_turn
                        next_turn_color = 'Black' if last_turn_color == 'White' else 'White'

                        # Calculate elapsed time
                        last_real_time_dt = datetime.strptime(last_real_time, "%Y-%m-%d %H:%M:%S")
                        current_real_time_dt = datetime.utcnow()
                        elapsed_time = (current_real_time_dt - last_real_time_dt).seconds

                        # Update the timers
                        if last_turn_color == 'White':
                            new_white_time = max(last_white_time - elapsed_time, 0)
                            new_black_time = last_black_time
                        else:
                            new_black_time = max(last_black_time - elapsed_time, 0)
                            new_white_time = last_white_time

                        # Check for timeout
                        result = None
                        if new_white_time <= 0:
                            result = "Black wins: White ran out of time."
                            new_white_time = 0
                        elif new_black_time <= 0:
                            result = "White wins: Black ran out of time."
                            new_black_time = 0

                        # Insert the new turn into the database
                        cursor.execute("""
                            INSERT INTO boards (GAME_NO, TURN_NO, TURN, BOARD, REAL_TIME, WHITE_TIME, BLACK_TIME)
                            VALUES (?, ?, ?, ?, datetime('now'), ?, ?)
                        """, (game_no, turn_no, next_turn_color, board, new_white_time, new_black_time))
                        conn.commit()

                        # Update the result in the games table if a timeout occurred
                        if result:
                            cursor.execute("""
                                UPDATE games SET RESULT = ? WHERE GAME_NO = ?
                            """, (result, game_no))
                            conn.commit()

                            print(f"DEBUG: Game result updated: {result}")

                            # Serve a page with the result message and links to /history.html and /index.html
                            result_page_content = f"""
                            <html>
                            <body style="background-color: #728FCE; color: white;">
                                <h1>{result}</h1>
                                <p><a href="/history.html" style="color: lightblue;">View Game History</a></p>
                                <p><a href="/index.html" style="color: lightblue;">Return to Home</a></p>
                            </body>
                            </html>
                            """
                            self.send_response(200)
                            self.send_header("Content-type", "text/html")
                            self.send_header("Content-length", len(result_page_content))
                            self.end_headers()
                            self.wfile.write(bytes(result_page_content, "utf-8"))
                            conn.close()
                            return

                        print(f"DEBUG: New turn inserted. Turn No: {turn_no}, Next Turn: {next_turn_color}")

                    else:
                        print("DEBUG: No previous turn found.")
                        self.send_response(404)
                        self.end_headers()
                        self.wfile.write(b"<html><body><h1>404 Turn Not Found</h1></body></html>")
                        conn.close()
                        return

                # Fetch current turn details for rendering
                cursor.execute("""
                    SELECT BOARD, WHITE_TIME, BLACK_TIME FROM boards WHERE GAME_NO = ? AND TURN_NO = ?
                """, (game_no, turn_no))
                current_turn = cursor.fetchone()

                if current_turn:
                    board, white_time, black_time = current_turn
                else:
                    board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
                    white_time = 300
                    black_time = 300

                def format_time(seconds):
                    minutes = seconds // 60
                    sec = seconds % 60
                    return f"{minutes}:{sec:02}"

                formatted_wtime = format_time(white_time)
                formatted_btime = format_time(black_time)
                next_turn = 'White' if turn_no % 2 == 0 else 'Black'

                # Serve opponent.html
                file_path = os.path.join('client', 'opponent.html')
                with open(file_path, 'r') as file:
                    content = file.read()
                    content = content.replace('{fen_string}', board)
                    content = content.replace('{game_no}', str(game_no))
                    content = content.replace('{turn_no}', str(turn_no))
                    content = content.replace('{formatted_wtime}', formatted_wtime)
                    content = content.replace('{formatted_btime}', formatted_btime)
                    content = content.replace('{next_turn}', next_turn)
                    content = content.replace('{wtime}', str(white_time))
                    content = content.replace('{btime}', str(black_time))

                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.send_header("Content-length", len(content))
                    self.end_headers()
                    self.wfile.write(bytes(content, "utf-8"))

                conn.close()
            except Exception as e:
                print(f"DEBUG: Error in /opponent.html: {e}")
                self.send_response(500)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>500 Internal Server Error</h1></body></html>")
        
        elif parsed.path in ['/check_new_turn']:
            try:
                query_params = dict(parse_qsl(parsed.query))
                game_no = query_params.get('game_no')
                turn_no = int(query_params.get('turn_no', 0))  # Default to 0 if missing
                board = query_params.get('board')

                conn = sqlite3.connect(DATABASE)
                cursor = conn.cursor()

                # Query for the next turn
                cursor.execute("""
                    SELECT COUNT(*) FROM boards WHERE GAME_NO = ?
                """, (game_no,))
                row = cursor.fetchone()[0]
                conn.close()

                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                if (row > turn_no):
                    next_turn_no = turn_no + 1
                    next_turn = 'White' if next_turn_no % 2 == 1 else 'Black'
                    self.wfile.write(bytes(json.dumps({
                        "turn_no": turn_no,
                        "row": row,
                        "new_turn_no": next_turn_no,
                        "board": board,
                        "game_no": game_no,
                        "next_turn": next_turn,
                        "board": board
                    }), "utf-8"))
                else:
                    self.wfile.write(bytes(json.dumps({"new_turn_no": None}), "utf-8"))

            except Exception as e:
                print(f"DEBUG: Error in /check_new_turn: {e}")
                self.send_response(500)
                self.end_headers()
                self.wfile.write(b"<html><body><h1>500 Internal Server Error</h1></body></html>")

        elif parsed.path in ['/waitpage.js']:
            try:
                file_path = './waitpage.js'  # Point to the server's root directory
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

        elif parsed.path in ['/check_black_handle']:
            # Parse the form data
            if self.command != 'POST':
                print(f"Unsupported HTTP method: {self.command}", flush=True)
                self.send_response(405)  # Method Not Allowed
                self.end_headers()
            else:    
                form = cgi.FieldStorage(fp=self.rfile,
                                headers=self.headers,
                                environ={'REQUEST_METHOD': 'POST',
                                        'CONTENT_TYPE': self.headers['Content-Type']})
                game_no = form.getvalue('game_no')
                
                print(f"Received /check_black_handle request with game_no: {game_no}", flush=True)
                
                conn = sqlite3.connect(DATABASE)
                cursor = conn.cursor()
                
                # Query the database for BLACK_HANDLE
                cursor.execute("SELECT BLACK_HANDLE FROM games WHERE GAME_NO = ?", (game_no,))
                result = cursor.fetchone()
                conn.close()
                
                # Send JSON response
                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                
                # Respond with BLACK_HANDLE value or None
                if result and result[0]:
                    self.wfile.write(bytes(json.dumps({"black_handle": result[0]}), "utf-8"))
                else:
                    self.wfile.write(bytes(json.dumps({"black_handle": None}), "utf-8"))

        
        elif parsed.path in ['/login.html']:
            form = cgi.FieldStorage(fp=self.rfile,
                            headers=self.headers,
                            environ={'REQUEST_METHOD': 'POST',
                                     'CONTENT_TYPE': self.headers['Content-Type']})
            handle = form.getvalue('handle')

            conn = sqlite3.connect(DATABASE)
            cursor = conn.cursor()

            # Check for games with an empty BLACK_HANDLE
            cursor.execute("SELECT GAME_NO FROM games WHERE BLACK_HANDLE IS NULL LIMIT 1")
            game = cursor.fetchone()

            if game:
                # Player 2 is joining an existing game
                game_no = game[0]
                cursor.execute("UPDATE games SET BLACK_HANDLE = ? WHERE GAME_NO = ?", (handle, game_no))
                conn.commit()

                # Add initial board setup for the game
                cursor.execute("""
                    INSERT INTO boards (GAME_NO, TURN_NO, TURN, BOARD, WHITE_TIME, BLACK_TIME)
                    VALUES (?, ?, ?, ?, ?, ?)
                """, (game_no, 1, 'White', 'start', GAME_TIME, GAME_TIME))  # Example board setup
                conn.commit()

                # Redirect Player 2 to opponent.html
                self.send_response(303)
                self.send_header("Location", f"/opponent.html?game_no={game_no}&turn_no=1")
                self.end_headers()

                # Notify Player 1 by including Player 2's handle in AJAX polling (handled via waitpage.js)
            else:
                # Player 1 is starting a new game
                cursor.execute("INSERT INTO games (WHITE_HANDLE) VALUES (?)", (handle,))
                game_no = cursor.lastrowid
                conn.commit()

                # Redirect Player 1 to the waiting page (login.html)
                self.send_response(303)
                self.send_header("Location", f"/login.html?game_no={game_no}")
                self.end_headers()

            conn.close()

        

        
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
        else:
            print(f"Unhandled POST request: {self.path}", flush=True)
            self.send_response(404)  # Respond with a 404 Not Found
            self.end_headers()

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