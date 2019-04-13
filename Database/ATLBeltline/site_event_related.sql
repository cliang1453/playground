-- ========22 Administrator Manage Transit ===================
-- Create Manage_Transit View
CREATE VIEW Manage_Transit AS
	SELECT Q.route, Q.type, Q.price, Q.connected_sites, count(*) as transit_logged
	From (SELECT distinct transit.route, transit.type, transit.price, count(*) as connected_sites 
		FROM transit, connect
		WHERE transit.type = connect.type and transit.route = connect.route
		GROUP BY transit.route, transit.type) as Q, take
	WHERE Q.route = take.route and Q.type = take.type and
	GROUP BY Q.route, Q.type;

-- Filter by type, route, constain site, price range 
SELECT *
FROM Manage_Transit
WHERE "Inman Park" in  (SELECT sitename 
			            FROM Manage_Transit, connect
                        WHERE Manage_Transit.route = connect.route and Manage_Transit.type = connect.type)
	  and route = "Relay" and type = "Bike" and  price >= 0 and price <= 10;


-- ==========23 Administrator Edit Transit =======================
-- Show connected sitename on specific transit
SELECT connect.sitename
FROM Manage_Transit, connect
WHERE Manage_Transit.type = "Bike" and Manage_Transit.route = "Relay" and Manage_Transit.price = 1.0 and 
      connect.type = Manage_Transit.type and connect.route = Manage_Transit.route

-- Edit transit connect site
INSERT INTO connect
SELECT "Westview Cemetery", "Bike", "Relay"
WHERE "Westview Cemetery" in (SELECT sitename FROM connect) 

DELETE FROM connect
WHERE sitename = "Westview Cemetery" and connect.type = "Bike" and connect.route = "Relay"

-- Edit route and price 
UPDATE transit
SET route = "Bike" and price = "2.5"
WHERE type = "Relay"

-- ==========24 Administrator Create Transit =======================
INSERT INTO transit
VALUE ("MARTA", "RED", 2.5)
INSERT INTO connect
VALUE ("Atlanta Beltline Center", "MARTA", "RED")
INSERT INTO connect
VALUE ("Inman Park", "MARTA", "RED")
INSERT INTO connect
VALUE ("Westview Cenmetery", "MARTA", "RED")

-- ==========25 Manage Event =======================
-- filter event
CREATE VIEW Manage_Event AS
	SELECT event.name, event.startdate, event.sitename, event.price, Q.staff_count, DATEDIFF(event.enddate, event.startdate)+1 as durations, Q.total_visits, Q.total_visits * event.price as total_revenue
	FROM event, 
		(SELECT Q_sc.name, Q_sc.startdate, Q_sc.sitename, Q_sc.staff_count, Q_tv.total_visits
		 FROM ((SELECT eventname as name, startdate, sitename, count(*) as staff_count FROM assign_to GROUP BY eventname, startdate, sitename) as Q_sc 
		  Join (SELECT ename as name, startdate, sitename, count(*) as total_visits FROM visitevent GROUP BY ename, startdate, sitename) as Q_tv
		  on Q_sc.name = Q_tv.name and Q_sc.startdate = Q_tv.startdate and Q_sc.sitename = Q_tv.sitename)) as Q
	WHERE event.name = Q.name and event.startdate = Q.startdate and event.sitename=Q.sitename

Select name, staff_count, durations, total_visits, total_revenue
FROM Manage_Event

-- ==========26 Manager View/Edit Event =========================
-- View staff assigned for current event and staff currently have no assignment
CREATE VIEW Current_Staff AS 
	(SELECT username as staff_assigned
    FROM staff
    WHERE username not in (SELECT distinct staffusername FROM assign_to))
    UNION
	(SELECT Q.staffusername as staff_assigned
	FROM (SELECT M.name, M.startdate, M.enddate, M.sitename, M.price, A.staffusername 
		FROM (assign_to as A join Manage_Event as M on A.eventname=M.name and A.startdate=M.startdate and A.sitename=M.sitename)) as Q
	WHERE Q.name="Bus Tour" and Q.price=25 and Q.startdate="2019-02-01" and Q.enddate="2019-02-01")

-- Display daily result for daily visit and daily revenue
SELECT startdate as date, total_visits/durations as daily_visit, total_revenue/durations as daily_revenue
FROM Manage_Event
WHERE name="Bus Tour" and price=25 and startdate="2019-02-01"

-- Insert new staff for current event
INSERT INTO assign_to
VALUE("Elizabeth Jones", "Bus Tour", "2019-02-01", "Inman Park")

-- Delete staff from current event
DELETE FROM assign_to
WHERE assign_to.eventname = "Bus Tour" and assign_to.staffusername = "Elizabeth Jones" and assign_to.startdate = "2019-02-01"

-- Update description
UPDATE event
SET description="new description"
WHERE name="Bus Tour" and startdate="2019-02-01" and sitename="Inman Park"

-- =============27 Create Event ==========================
INSERT INTO event 
VALUE ("Arboretum Walking Tour", "2019-02-01", "sitename", "2019-02-02", 0, 20, 1, "new description")
INSERT INTO assign_to
VALUE ("Elizabeth Jones", "Arboretum Walking Tour", "2019-02-01", "sitename")



    






      




