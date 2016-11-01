-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later)
-- ?? represents filling in with actual value

-- Needed GETS
-- GET /users (all fields)
SELECT * FROM sbacsDb.Users
WHERE User_Id = ??;

-- GET /users (no user_id)
SELECT * FROM sbacsDb.Users
WHERE Name like '%??' AND Rights like '%??';

-- Needed PUTS / DELTES
-- PUT /users
INSERT INTO sbacsDb.Users (Name, Rights) VALUES (??, ??)
ON DUPLICATE KEY UPDATE Name=??, Rights=??;
-- TODO: THEN Need to get the id of the inserted/updated row

-- DELETE /users
DELETE FROM sbacsDb.Users
WHERE User_Id = ??;


-- Needed POSTS
-- POST /users (with all fields, remove other fields)
UPDATE sbacsDb.Users
SET Name = ??, Rights = ??
WHERE User_Id = ??;