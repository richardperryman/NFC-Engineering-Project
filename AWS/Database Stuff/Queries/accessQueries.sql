-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later

-- GET /access Needs ALL OF FOLLOWING:

-- get the list of identities that are associated with the lock
SELECT * FROM sbacsDb.Registrations
WHERE Lock_Id = ??;

-- Then get the needed authenticators for each identity from above
SELECT * FROM sbacsDb.Authenticators
WHERE Identity_Id = ??;

-- Compare each list until one matches

-- IF (found match)
SELECT lockKey FROM Locks
WHERE Lock_Id = ??;
-- THEN return the encrypted key

-- ELSE (no match)
-- THEN return 401 error with message