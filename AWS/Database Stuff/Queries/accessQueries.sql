-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later

-- GET /access Needs ALL OF FOLLOWING:

-- Get the needed authenticators for each identity on the given lock from above
SELECT a.Auth_Id, a.AuthType, a.AuthKey, a.AuthSalt
FROM sbacsDb.Authenticators as a, sbacsDb.IdentityToAuth as ita, sbacsDb.Registrations as r
WHERE r.Lock_Id = ?? AND r.Identity_Id = ita.Identity_Id AND ita.Auth_Id = a.Auth_Id;

-- Compare each list until one matches

-- IF (found match)
SELECT lockKey FROM Locks
WHERE Lock_Id = ??;
-- THEN return the encrypted key

-- ELSE (no match)
-- THEN return 401 error with message