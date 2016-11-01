-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later

-- GET /authenticators (with auth_id)
SELECT * FROM sbacsDb.Authenticators
WHERE Auth_Id = ??;

-- GET /authenticators (without auth_id, with identity_id)
SELECT * FROM sbacsDb.Authenticators
WHERE Identity_Id = ??;

-- GET /authenticators (with only user_id)
SELECT a.Auth_Id, a.Identity_Id, a.AuthType, a.AuthKey
FROM sbacsDb.Authenticators AS a, sbacsDb.Identities AS i
WHERE i.User_Id = ?? AND i.Identity_Id = a.Identity_Id;


-- Needed PUTS / DELTES
-- PUT /authenticators
INSERT INTO sbacsDb.Authenticators (Identity_Id, AuthType, AuthKey) VALUES (??, ??, ??)
ON DUPLICATE KEY UPDATE Identity_Id=??, AuthType=??, AuthKey=??;
-- TODO: THEN Need to get the id of the inserted/updated row

-- DELETE /authenticators
DELETE FROM sbacsDb.Authenticators
WHERE Auth_Id = ??;


-- Needed POSTS
-- POST /authenticators (always all fields)
UPDATE sbacsDb.Authenticators
SET AuthType = ??, AuthKey = ??
WHERE Auth_Id = ??;



