import phylib;
import sqlite3; #For SQL stuff
import os;
import math;

HEADER = """<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
 "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg width="700" height="1375" viewBox="-25 -25 1400 2750"
 xmlns="http://www.w3.org/2000/svg"
 xmlns:xlink="http://www.w3.org/1999/xlink">
 <rect width="1350" height="2700" x="0" y="0" fill="#C0D0C0" />""";
FOOTER = """</svg>\n""";

################################################################################
# import constants from phylib to global varaibles
BALL_RADIUS = phylib.PHYLIB_BALL_RADIUS;
BALL_DIAMETER = phylib.PHYLIB_BALL_DIAMETER;

HOLE_RADIUS = phylib.PHYLIB_HOLE_RADIUS;
TABLE_LENGTH = phylib.PHYLIB_TABLE_LENGTH;
TABLE_WIDTH = phylib.PHYLIB_TABLE_WIDTH;

SIM_RATE = phylib.PHYLIB_SIM_RATE;
VEL_EPSILON = phylib.PHYLIB_VEL_EPSILON;

DRAG = phylib.PHYLIB_DRAG;
MAX_TIME = phylib.PHYLIB_MAX_TIME;

MAX_OBJECTS = phylib.PHYLIB_MAX_OBJECTS;

# A3 constants
FRAME_INTERVAL = 0.01;

# add more here

################################################################################
# the standard colours of pool balls
# if you are curious check this out:  
# https://billiards.colostate.edu/faq/ball/colors/

BALL_COLOURS = [ 
    "WHITE",
    "YELLOW",
    "BLUE",
    "RED",
    "PURPLE",
    "ORANGE",
    "GREEN",
    "BROWN",
    "BLACK",
    "LIGHTYELLOW",
    "LIGHTBLUE",
    "PINK",             # no LIGHTRED
    "MEDIUMPURPLE",     # no LIGHTPURPLE
    "LIGHTSALMON",      # no LIGHTORANGE
    "LIGHTGREEN",
    "SANDYBROWN",       # no LIGHTBROWN 
    ];

################################################################################
class Coordinate( phylib.phylib_coord ):
    """
    This creates a Coordinate subclass, that adds nothing new, but looks
    more like a nice Python class.
    """
    pass;


################################################################################
class StillBall( phylib.phylib_object ):
    """
    Python StillBall class.
    """

    def __init__( self, number, pos ):
        """
        Constructor function. Requires ball number and position (x,y) as
        arguments.
        """

        # this creates a generic phylib_object
        phylib.phylib_object.__init__( self, 
                                       phylib.PHYLIB_STILL_BALL, 
                                       number, 
                                       pos, None, None, 
                                       0.0, 0.0 );
      
        # this converts the phylib_object into a StillBall class
        self.__class__ = StillBall;


    # add an svg method here
    def svg( self ):
        if(BALL_COLOURS[self.obj.still_ball.number]=="WHITE"):
            return """ <circle id="cue-ball" cx="%d" cy="%d" r="%d" fill="%s" />\n""" % (self.obj.still_ball.pos.x, self.obj.still_ball.pos.y, BALL_RADIUS, BALL_COLOURS[self.obj.still_ball.number])
        return """ <circle cx="%d" cy="%d" r="%d" fill="%s" />\n""" % (self.obj.still_ball.pos.x, self.obj.still_ball.pos.y, BALL_RADIUS, BALL_COLOURS[self.obj.still_ball.number])
    
################################################################################
class RollingBall( phylib.phylib_object ):
    """
    Python RollingBall class.
    """

    def __init__( self, number, pos, vel, acc ):
        """
        Constructor function. Requires ball number, position (x,y) as
        velocity and acceleration as arguments.
        """

        # this creates a generic phylib_object
        phylib.phylib_object.__init__( self, 
                                       phylib.PHYLIB_ROLLING_BALL, 
                                       number, 
                                       pos, vel, acc,
                                       0.0, 0.0 );
      
        # this converts the phylib_object into a RollingBall class
        self.__class__ = RollingBall;


    # add an svg method here
    def svg( self ):
        if(BALL_COLOURS[self.obj.rolling_ball.number]=="WHITE"):
            return """ <circle id="cue-ball" cx="%d" cy="%d" r="%d" fill="%s" />\n""" % (self.obj.rolling_ball.pos.x, self.obj.rolling_ball.pos.y, BALL_RADIUS, BALL_COLOURS[self.obj.rolling_ball.number])
        return """ <circle cx="%d" cy="%d" r="%d" fill="%s" />\n""" % (self.obj.rolling_ball.pos.x, self.obj.rolling_ball.pos.y, BALL_RADIUS, BALL_COLOURS[self.obj.rolling_ball.number])
    
################################################################################
class Hole( phylib.phylib_object ):
    """
    Python Hole class.
    """

    def __init__( self, pos ):
        """
        Constructor function. Requires ball position (x,y) as arguments.
        """

        # this creates a generic phylib_object
        phylib.phylib_object.__init__( self, 
                                       phylib.PHYLIB_HOLE, 
                                       None, 
                                       pos, None, None, 
                                       0.0, 0.0 );
      
        # this converts the phylib_object into a Hole class
        self.__class__ = Hole;


    # add an svg method here
    def svg( self ):
        return """ <circle cx="%d" cy="%d" r="%d" fill="black" />\n""" % (self.obj.hole.pos.x, self.obj.hole.pos.y, HOLE_RADIUS)
    
################################################################################
class HCushion( phylib.phylib_object ):
    """
    Python HCushion class.
    """

    def __init__( self, y ):
        """
        Constructor function. Requires y as argument.
        """

        # this creates a generic phylib_object
        phylib.phylib_object.__init__( self, 
                                       phylib.PHYLIB_HCUSHION, 
                                       None, 
                                       None, None, None, 
                                       0.0, y );
      
        # this converts the phylib_object into a HCushion class
        self.__class__ = HCushion;


    # add an svg method here
    def svg(self):
        y = -25 if self.obj.hcushion.y == 0 else 2700
        return """ <rect width="1400" height="25" x="-25" y="%d" fill="darkgreen" />\n""" % (y)

   
    
################################################################################
class VCushion( phylib.phylib_object ):
    """
    Python VCushion class.
    """

    def __init__( self, x ):
        """
        Constructor function. Requires x as argument.
        """

        # this creates a generic phylib_object
        phylib.phylib_object.__init__( self, 
                                       phylib.PHYLIB_VCUSHION, 
                                       None, 
                                       None, None, None, 
                                       x, 0.0 );
      
        # this converts the phylib_object into a VCushion class
        self.__class__ = VCushion;


    # add an svg method here
    def svg(self):
        x = -25 if self.obj.vcushion.x == 0 else 1350
        return """ <rect width="25" height="2750" x="%d" y="-25" fill="darkgreen" />\n""" % (x)
    

################################################################################

class Table( phylib.phylib_table ):
    """
    Pool table class.
    """

    def __init__( self ):
        """
        Table constructor method.
        This method call the phylib_table constructor and sets the current
        object index to -1.
        """
        phylib.phylib_table.__init__( self );
        self.current = -1;

    def __iadd__( self, other ):
        """
        += operator overloading method.
        This method allows you to write "table+=object" to add another object
        to the table.
        """
        self.add_object( other );
        return self;

    def __iter__( self ):
        """
        This method adds iterator support for the table.
        This allows you to write "for object in table:" to loop over all
        the objects in the table.
        """
        return self;

    def __next__( self ):
        """
        This provides the next object from the table in a loop.
        """
        self.current += 1;  # increment the index to the next object
        if self.current < MAX_OBJECTS:   # check if there are no more objects
            return self[ self.current ]; # return the latest object

        # if we get there then we have gone through all the objects
        self.current = -1;    # reset the index counter
        raise StopIteration;  # raise StopIteration to tell for loop to stop

    def __getitem__( self, index ):
        """
        This method adds item retreivel support using square brackets [ ] .
        It calls get_object (see phylib.i) to retreive a generic phylib_object
        and then sets the __class__ attribute to make the class match
        the object type.
        """
        result = self.get_object( index ); 
        if result==None:
            return None;
        if result.type == phylib.PHYLIB_STILL_BALL:
            result.__class__ = StillBall;
        if result.type == phylib.PHYLIB_ROLLING_BALL:
            result.__class__ = RollingBall;
        if result.type == phylib.PHYLIB_HOLE:
            result.__class__ = Hole;
        if result.type == phylib.PHYLIB_HCUSHION:
            result.__class__ = HCushion;
        if result.type == phylib.PHYLIB_VCUSHION:
            result.__class__ = VCushion;
        return result;

    def __str__( self ):
        """
        Returns a string representation of the table that matches
        the phylib_print_table function from A1Test1.c.
        """
        result = "";    # create empty string
        result += "time = %6.1f;\n" % self.time;    # append time
        for i,obj in enumerate(self): # loop over all objects and number them
            result += "  [%02d] = %s\n" % (i,obj);  # append object description
        return result;  # return the string

    def segment( self ):
        """
        Calls the segment method from phylib.i (which calls the phylib_segment
        functions in phylib.c.
        Sets the __class__ of the returned phylib_table object to Table
        to make it a Table object.
        """

        result = phylib.phylib_table.segment( self );
        if result:
            result.__class__ = Table;
            result.current = -1;
        return result;

    # add svg method here
    def svg(self):
        svg_content = HEADER
        
        for obj in self:
            if obj is not None:
                svg_content += obj.svg()
        
        svg_content += FOOTER 
        return svg_content
    
    # Additional function for A3
    def roll(self, t):
        new = Table()
        for ball in self:
            if isinstance(ball, RollingBall):
                # create a new ball with the same number as the old ball
                new_ball = RollingBall(ball.obj.rolling_ball.number,
                                    Coordinate(0, 0),
                                    Coordinate(0, 0),
                                    Coordinate(0, 0))
                # compute where it rolls to
                phylib.phylib_roll(new_ball, ball, t)
                # add ball to table
                new += new_ball
            if isinstance(ball, StillBall):
                # create a new ball with the same number and pos as the old ball
                new_ball = StillBall(ball.obj.still_ball.number,
                                    Coordinate(ball.obj.still_ball.pos.x,
                                                ball.obj.still_ball.pos.y))
                # add ball to table
                new += new_ball
        # return table
        return new
    
    # Helper function(s) for A3
    # HELPER: Sets the cue ball (for shoot function)
    def cueBall( self, xvel, yvel ):
        for ball in self:
            if isinstance( ball, StillBall ) and ball.obj.still_ball.number == 0:
                    # Sets the still ball to a rolling ball and sets the ball num to 0
                    ball.type = phylib.PHYLIB_ROLLING_BALL
                    ball.obj.rolling_ball.num = 0
                    # updates the positions accordingly
                    ball.obj.rolling_ball.pos = Coordinate(ball.obj.still_ball.pos.x, ball.obj.still_ball.pos.y)
                    # will calculate the new acc and also set it accordingly
                    self.setBall(ball, xvel, yvel)
            elif isinstance( ball, RollingBall ) and ball.obj.rolling_ball.number == 0:
                    # will calculate the new acc and also set it accordingly
                    self.setBall(ball, xvel, yvel)
    
    # HELPER: Calculates the acceleration (for cueBall function)
    def calculateAccShoot(self, xvel, yvel):
        speed = math.sqrt(xvel * xvel + yvel * yvel)
        if speed > VEL_EPSILON:
            xacc = (-1.0) * xvel / speed * DRAG
            yacc = (-1.0) * yvel / speed * DRAG
        else:
            xacc = 0
            yacc = 0
        return Coordinate(xacc, yacc)
    
    # HELPER: Sets the motion of the balls (for cueBall function)
    def setBall(self, ball, xvel, yvel):
        # sets the new vel for the ball
        ball.obj.rolling_ball.vel = Coordinate(xvel, yvel)
        # sets the new acc for the ball
        ball.obj.rolling_ball.acc = self.calculateAccShoot(xvel, yvel)
        
        
        
        
############
#A3 Code
class Database():
    # constructor, for the database
    def __init__( self, reset=False ):
        self.db_path = "phylib.db"
        if reset and os.path.exists(self.db_path):
            os.remove(self.db_path)
        self.conn = sqlite3.connect(self.db_path)
        self.createDB()
        
        
    # creates the database tables    
    def createDB( self ):
        cur = self.conn.cursor()
        # Ball table
        cur.execute('''CREATE TABLE IF NOT EXISTS Ball (
            BALLID INTEGER PRIMARY KEY AUTOINCREMENT,
            BALLNO INTEGER NOT NULL,
            XPOS FLOAT NOT NULL,
            YPOS FLOAT NOT NULL,
            XVEL FLOAT,
            YVEL FLOAT)''')
        
        # TTable table
        cur.execute('''CREATE TABLE IF NOT EXISTS TTable (
            TABLEID INTEGER PRIMARY KEY AUTOINCREMENT,
            TIME FLOAT NOT NULL)''')
        
        # BallTable table
        cur.execute('''CREATE TABLE IF NOT EXISTS BallTable (
            BALLID INTEGER NOT NULL,
            TABLEID INTEGER NOT NULL,
            FOREIGN KEY(BALLID) REFERENCES Ball(BALLID),
            FOREIGN KEY(TABLEID) REFERENCES TTable(TABLEID))''')

        # Shot table
        cur.execute('''CREATE TABLE IF NOT EXISTS Shot (
            SHOTID INTEGER PRIMARY KEY AUTOINCREMENT,
            PLAYERID INTEGER NOT NULL,
            GAMEID INTEGER NOT NULL,
            FOREIGN KEY(PLAYERID) REFERENCES Player(PLAYERID),
            FOREIGN KEY(GAMEID) REFERENCES Game(GAMEID))''')

        # TableShot table
        cur.execute('''CREATE TABLE IF NOT EXISTS TableShot (
            TABLEID INTEGER NOT NULL,
            SHOTID INTEGER NOT NULL,
            FOREIGN KEY(TABLEID) REFERENCES TTable(TABLEID),
            FOREIGN KEY(SHOTID) REFERENCES Shot(SHOTID))''')

        # Game table
        cur.execute('''CREATE TABLE IF NOT EXISTS Game (
            GAMEID INTEGER PRIMARY KEY AUTOINCREMENT,
            GAMENAME VARCHAR(64) NOT NULL)''')

        # Player table
        cur.execute('''CREATE TABLE IF NOT EXISTS Player (
            PLAYERID INTEGER PRIMARY KEY AUTOINCREMENT,
            GAMEID INTEGER NOT NULL,
            PLAYERNAME VARCHAR(64) NOT NULL,
            FOREIGN KEY(GAMEID) REFERENCES Game(GAMEID))''')
        
        self.conn.commit()

    
    # this reads the table (duh)
    def readTable(self, tableID):
        # Gets all the ball data needed for reading
        balls = self.getBallData(tableID) 
        if not balls:
            return None

        # Sets the time
        table = Table()
        table.time = balls[0][-1]  # Set the time from the first row, assuming all rows have the same table time

        # Will create the ball according to its values
        for row in balls:    
            ball = self.createBallRT(ballNum=row[1], xpos=row[2], ypos=row[3], xvel=row[4], yvel=row[5])
            table += ball
        
        return table

    # Helper function(s) for A3
    # HELPER: Gets the ball data (for readTable function)
    def getBallData(self, tableID):
        cur = self.conn.cursor()
        # Adjusts the ID for SQL index
        tid = tableID + 1
        balls = cur.execute('''SELECT Ball.*, TTable.TIME FROM Ball
        JOIN BallTable ON Ball.BALLID = BallTable.BALLID
        JOIN TTable ON BallTable.TABLEID = TTable.TABLEID
        WHERE TTable.TABLEID = ?;''', (tid,)).fetchall()
        cur.close()
        return balls
    
    # HELPER: Calculates the ball acc (RT) (for readTable function)
    def calculateAccRT(self, xvel, yvel):
        speed = math.sqrt(xvel * xvel + yvel * yvel)
        if speed > VEL_EPSILON:
            xacc = (-1.0) * xvel / speed * DRAG
            yacc = (-1.0) * yvel / speed * DRAG
        else:
            xacc = 0
            yacc = 0
        return Coordinate(xacc, yacc)
    
    # HELPER: creates a rolling ball or still ball (for readTable function)
    def createBallRT(self, ballNum, xpos, ypos, xvel, yvel):
        if xvel is None and yvel is None:
            # for still ball
            return StillBall(ballNum, Coordinate(xpos, ypos))
        else:
            # for rolling ball
            acceleration = self.calculateAccRT(xvel, yvel)
            return RollingBall(ballNum, Coordinate(xpos, ypos), Coordinate(xvel, yvel), acceleration)


    # this writes the table (duh)
    def writeTable(self, table):
        cur = self.conn.cursor()
        # Insert into TTable and get TABLEID
        cur.execute('''INSERT INTO TTable (TIME) VALUES (?)''', (table.time,))
        tid = cur.lastrowid

        # Only insert balls into Ball and BallTable, ignore cushions and holes
        for ball in table:
            if isinstance(ball, (StillBall, RollingBall)):
                self.insertBall(cur, ball, tid)
        
        self.conn.commit()
        return tid - 1  # Adjusting to preferred numbering
    
    # Helper function(s) for A3
    # HELPER: inserts the ball into the table(for writeTable function)
    def insertBall(self, cur, ball, tid):
        # if ball is a still ball
        if isinstance(ball, StillBall):
            ball_data = (ball.obj.still_ball.number, ball.obj.still_ball.pos.x, ball.obj.still_ball.pos.y)
            cur.execute('''INSERT INTO Ball (BALLNO, XPOS, YPOS) VALUES (?, ?, ?)''', ball_data)
            ballId = cur.lastrowid
            # inserts the ball into the table
            cur.execute('''INSERT INTO BallTable (BALLID, TABLEID) VALUES (?, ?)''', (ballId, tid))
        # if ball is a rolling ball
        elif isinstance(ball, RollingBall):
            ball_data = (ball.obj.rolling_ball.number, ball.obj.rolling_ball.pos.x, ball.obj.rolling_ball.pos.y, ball.obj.rolling_ball.vel.x, ball.obj.rolling_ball.vel.y)
            cur.execute('''INSERT INTO Ball (BALLNO, XPOS, YPOS, XVEL, YVEL) VALUES (?, ?, ?, ?, ?)''', ball_data)
            ballId = cur.lastrowid
            # inserts the ball into the table
            cur.execute('''INSERT INTO BallTable (BALLID, TABLEID) VALUES (?, ?)''', (ballId, tid))

    #bunch of helper functions to just fix the ID alignment on the database
    def adjustedGameID (self):
        cur = self.conn.cursor()
        cur.execute("SELECT MAX(GAMEID) FROM GAME")
        gameID = cur.fetchone()[0]
        cur.close()
        return gameID - 1
    
    def adjustedTableShot(self, shotID):
        cur = self.conn.cursor()
        cur.execute("SELECT COUNT(TABLEID) FROM TableShot WHERE SHOTID = ?", (shotID,))
        tableNum = cur.fetchone()[0]
        cur.close()
        return tableNum

    def adjustedTableID(self):
        cur = self.conn.cursor()
        cur.execute("SELECT MAX(TABLEID) FROM TTABLE")
        tableID = cur.fetchone()[0]
        cur.close()
        return tableID - 1

    def adjustedShotID(self):
        cur = self.conn.cursor()
        cur.execute("SELECT MAX(SHOTID) FROM SHOT")
        shotID = cur.fetchone()[0]
        cur.close()
        return shotID - 1

    
    # yeah close
    def close ( self ):
        self.conn.commit()
        self.conn.close()
   
    
    
    
      
class Game(): 
    # loads or creates game
    def __init__(self, gameID=None, gameName=None, player1Name=None, player2Name=None):
        self.db = Database()  # Assumes a Database instance can be initialized here
        
        # checks for existing game
        if gameID is not None and gameName is None and player1Name is None and player2Name is None:
            self.getGame(gameID)
        # checks if no existing game, will create one
        elif gameID is None and all([gameName, player1Name, player2Name]):
            self.setGame(gameName, player1Name, player2Name)
        else:
            raise TypeError("Invalid arguments provided.")
        
    # Helper function(s) for A3
    # HELPER: loads in an existing game (for __init__ function)    
    def getGame(self, gameID):
        cur = self.db.conn.cursor()
        # adjust the index for sql purposes
        indexAdjustedGameID = gameID + 1
        cur.execute('''SELECT GAMENAME FROM Game WHERE GAMEID = ?''', (indexAdjustedGameID,))
        result = cur.fetchone()
        if result:
            self.gameName = result[0]
        
        cur.execute('''SELECT PLAYERNAME FROM Player WHERE GAMEID = ? ORDER BY PLAYERID ASC''', (indexAdjustedGameID,))
        players = cur.fetchall()
        
        # validates players
        if players:
            self.player1Name = players[0][0]
            if len(players) > 1:
                self.player2Name = players[1][0]
            else:
                self.player2Name = None

        self.gameID = indexAdjustedGameID
        
    # HELPER: creates a new game (for __init__ function)
    def setGame(self, gameName, player1Name, player2Name):
        cur = self.db.conn.cursor()
        # insert game name into game tabe
        cur.execute('''INSERT INTO Game (GAMENAME) VALUES (?)''', (gameName,))
        self.gameID = cur.lastrowid
        
        # adding the players to the table
        cur.execute('''INSERT INTO Player (GAMEID, PLAYERNAME) VALUES (?, ?)''', (self.gameID, player1Name))
        cur.execute('''INSERT INTO Player (GAMEID, PLAYERNAME) VALUES (?, ?)''', (self.gameID, player2Name))
        
        self.gameName = gameName
        self.player1Name = player1Name
        self.player2Name = player2Name
        self.db.conn.commit()
    
    
    # pew pew
    def shoot( self, gameName, playerName, table, xvel, yvel ):
        # cue ball movement
        table.cueBall(xvel, yvel)
        
        cur = self.db.conn.cursor()
        # Gets the player id using the name and game id
        playerID = cur.execute("""SELECT PLAYERID FROM Player WHERE PLAYERNAME = ? AND GAMEID = ?""", (playerName, self.gameID)).fetchone()
        if playerID is None:
            return
        #gets player id from result
        playerID = playerID[0]
        
        cur.execute("""INSERT INTO Shot (PLAYERID, GAMEID) VALUES (?, ?)""", (playerID, self.gameID))
        shotID = cur.lastrowid

        svgIndex = 0
               
        while table:
            startTime = table.time
            next_segment = table.segment() 

            if next_segment is None:
                break

            sub_timediff = next_segment.time - startTime
            value = sub_timediff / FRAME_INTERVAL
            frameCount = math.floor(value)


            for j in range(frameCount):
                newTime = FRAME_INTERVAL * j
                newTable = table.roll(newTime)
                newTable.time = startTime + newTime
                
                
                tableID = self.db.writeTable(newTable)
                svgCreation(newTable, svgIndex)
                svgIndex += 1 
                self.insertRecord(tableID, shotID)

            table = next_segment 

        self.db.conn.commit()
        
    # Helper function(s) for A3
    # HELPER: inserts into table (for shoot function)        
    def insertRecord(self, tableID, shotID):
        query = """INSERT INTO TableShot (TABLEID, SHOTID) VALUES (?, ?)"""
        cur = self.db.conn.cursor()
        cur.execute(query, (tableID, shotID))
        cur.close()
        
def svgCreation(table, index):
    filename = f"table-{index}.svg"
    svg_content = table.svg()
    with open(filename, 'w') as file:
        file.write(svg_content)

