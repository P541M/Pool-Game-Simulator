import sys; # used to get argv
import cgi; # used to parse Mutlipart FormData 
            # this should be replace with multipart in the future
import os; #needed to delete files
import math;
import Physics
from Physics import Database, Game;
# web server parts
from http.server import HTTPServer, BaseHTTPRequestHandler;

# used to parse the URL and extract form data for GET requests
from urllib.parse import urlparse, parse_qsl;
import json # for javascript

# handler for our web-server - handles both GET and POST requests
class MyHandler( BaseHTTPRequestHandler ):
    def do_GET(self):
        # parse the URL to get the path and form data
        parsed  = urlparse( self.path );
        print("Parsed path: ", parsed.path)
        # check if the web-pages matches the list
        if parsed.path in [ '/startingPage.html' ]:

            # retreive the HTML file
            fp = open( '.'+self.path );
            content = fp.read();

            # generate the headers
            self.send_response( 200 ); # OK
            self.send_header( "Content-type", "text/html" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            # send it to the broswer
            self.wfile.write( bytes( content, "utf-8" ) );
            fp.close();

        elif parsed.path.startswith("/table-") and parsed.path.endswith(".svg"):        
            # this one is different because its an image file
            
            # retreive the SVG file (binary, not text file)
            fp = open( '.'+self.path, 'rb' );
            content = fp.read();

            self.send_response( 200 ); # OK
            # notice the change in Content-type
            self.send_header( "Content-type", "image/svg+xml" );
            self.send_header( "Content-length", len( content ) );
            self.end_headers();

            self.wfile.write( content );    # binary file
            fp.close();
            
        #for linking javascript files
        elif parsed.path.endswith('.js'):
            file_path = '.' + parsed.path
            with open(file_path, 'r') as file:
                content = file.read()
            self.send_response(200)
            self.send_header("Content-type", "application/javascript")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))
            file.close()
            
        #for linking css files
        elif parsed.path.endswith('.css'):
            file_path = '.' + parsed.path
            with open(file_path, 'r') as file:
                content = file.read()
            self.send_response(200)
            self.send_header("Content-type", "text/css")
            self.send_header("Content-length", len(content))
            self.end_headers()
            self.wfile.write(bytes(content, "utf-8"))
            file.close()
        else:
            # generate 404 for GET requests that aren't the 3 files above
            self.send_response( 404 );
            self.end_headers();
            self.wfile.write( bytes( "404: %s not found" % self.path, "utf-8" ) );

    def do_POST(self):
        parsed = urlparse(self.path)
        
        if parsed.path == "/send-data":
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data.decode('utf-8'))
            
            response = {"status": "success", "message": "Shot data processed successfully."}
            self.send_response(200)
            self.send_header("Content-type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps(response).encode('utf-8'))
            
            dx = float(data['x2']) - float(data['x1'])
            dy = float(data['y2']) - float(data['y1'])
            
            db = self.initDB(reset=False)
            table_id = db.adjustedTableID()
            game_id = db.adjustedGameID()
            table = db.readTable(table_id)
            game = Physics.Game(gameID = game_id)
            game.shoot(game.player1Name, game.player2Name, table, dx, dy)
            shotID = db.adjustedShotID()
            tableNum = db.adjustedTableShot(shotID)
        else:
            self.cleanSVGs()
        
            form = cgi.FieldStorage(fp=self.rfile, headers=self.headers,
                                    environ={'REQUEST_METHOD': 'POST', 'CONTENT_TYPE': self.headers['Content-Type']})
            
            table = Physics.Table()
            
            player1 = form["player1_name"].value
            player2 = form["player2_name"].value
            table += Physics.StillBall(0, Physics.Coordinate(675.0, 2025.0))
            table += Physics.StillBall(1, Physics.Coordinate(675.0, 1000.0))
            table += Physics.StillBall(2, Physics.Coordinate(640.0, 940.0))
            table += Physics.StillBall(3, Physics.Coordinate(710.0, 940.0))
            table += Physics.StillBall(4, Physics.Coordinate(675.0, 880.0))
            table += Physics.StillBall(5, Physics.Coordinate(605.0, 880.0))
            table += Physics.StillBall(6, Physics.Coordinate(745.0, 880.0))
            table += Physics.StillBall(7, Physics.Coordinate(570.0, 820.0))
            table += Physics.StillBall(8, Physics.Coordinate(640.0, 820.0))
            table += Physics.StillBall(9, Physics.Coordinate(710.0, 820.0))
            table += Physics.StillBall(10, Physics.Coordinate(780.0, 820.0))
            table += Physics.StillBall(11, Physics.Coordinate(535.0, 760.0))
            table += Physics.StillBall(12, Physics.Coordinate(605.0, 760.0))
            table += Physics.StillBall(13, Physics.Coordinate(675.0, 760.0))
            table += Physics.StillBall(14, Physics.Coordinate(745.0, 760.0))
            table += Physics.StillBall(15, Physics.Coordinate(815.0, 760.0))
            
            db = Physics.Database( reset=True )
            tableID = db.writeTable(table)
            game = Physics.Game(gameName = "my game", player1Name=player1, player2Name = player2)
            
            file = open('startingTable.html')
            table = file.read()
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(table))
            self.end_headers()
            self.wfile.write(bytes(table, "utf-8"))
            file.close()

    def cleanSVGs(self):
        for filename in os.listdir('.'):
            if filename.endswith('.svg'):
                os.remove(filename)

    def initDB(self, reset):
        db = Physics.Database(reset=reset)
        db.createDB()
        return db
    
if __name__ == "__main__":
    httpd = HTTPServer( ( 'localhost', int(sys.argv[1]) ), MyHandler );
    print( "Server listing in port:  ", int(sys.argv[1]) );
    httpd.serve_forever();