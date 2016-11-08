-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later

-- GET /authenticators (with auth_id)
SELECT * FROM sbacsDb.Authenticators
WHERE Auth_Id = ??;

-- GET /authenticators (without auth_id, with identity_id)
SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt
FROM sbacsDb.IdentityToAuth as i, sbacsDb.Authenticators as a
WHERE i.Identity_Id = ??;

-- GET /authenticators (with only user_id)
SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt
FROM sbacsDb.Identities AS i, sbacsDb.IdentityToAuth as ita, sbacsDb.Authenticators as a
WHERE i.User_Id = ?? AND i.Identity_Id = ita.Identity_Id AND ita.Auth_Id = a.Auth_Id;


-- Needed PUTS / DELTES
-- PUT /authenticators
INSERT INTO sbacsDb.Authenticators (AuthType, AuthKey, AuthSalt) VALUES (??, ??, ??);
-- TODO: THEN Need to get the id of the inserted/updated row

-- DELETE /authenticators
DELETE FROM sbacsDb.Authenticators
WHERE Auth_Id = ??;


-- Needed POSTS
-- POST /authenticators (always all fields)
UPDATE sbacsDb.Authenticators
SET AuthType = ??, AuthKey = ??, AuthSalt = ??
WHERE Auth_Id = ??;



