-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later

-- Needed PUTS / DELTES
-- PUT /access/management
INSERT INTO sbacsDb.Registrations (Identity_Id, Lock_Id) VALUES (??, ??)
ON DUPLICATE KEY UPDATE Identity_Id=??, Lock_Id=??;
-- TODO: THEN Need to get the id of the inserted/updated row

-- DELETE /access/management
DELETE FROM sbacsDb.Registrations
WHERE Lock_Id = ?? AND Identity_Id = ??;

