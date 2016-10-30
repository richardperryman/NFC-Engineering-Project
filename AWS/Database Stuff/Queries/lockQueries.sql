-- Basic Queries for Controllers to use
--(will need to be made into prepared statements later

-- GET /locks (with lock_id)
SELECT * FROM sbacsDb.Locks
WHERE Lock_Id = ??;

-- GET /locks (without lock_id)
SELECT * FROM sbacsDb.Locks
WHERE Name like '%??' AND BelongsTo like '%??';

-- Needed PUTS / DELTES
-- PUT /locks
INSERT INTO sbacsDb.Locks (Name, BelongsTo, lockKey) VALUES (??, ??, ??)
ON DUPLICATE KEY UPDATE Name=??, BelongsTo=??, lockKey=??;
-- TODO: THEN Need to get the id of the inserted/updated row

-- DELETE /locks
DELETE FROM sbacsDb.Locks
WHERE Lock_Id = ??;


-- Needed POSTS
-- POST /locks (with all fields, remove other fields)
UPDATE sbacsDb.Locks
SET Name = ??, BelongsTo = ??
WHERE Lock_Id = ??;