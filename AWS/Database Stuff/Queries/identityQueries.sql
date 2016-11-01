-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later

-- GET /identities (with identity_id)
SELECT * FROM sbacsDb.Identities
WHERE Identity_Id = ??;

-- GET /identities (without identity_id)
SELECT * FROM sbacsDb.Identities
WHERE User_Id = ??;

-- Needed PUTS / DELTES
-- PUT /identities
INSERT INTO sbacsDb.Identities (User_Id, Name) VALUES (??, ??)
ON DUPLICATE KEY UPDATE User_Id=??, Name=??;
-- TODO: THEN Need to get the id of the inserted/updated row

-- DELETE /identities
DELETE FROM sbacsDb.Identities
WHERE Identity_Id = ??;