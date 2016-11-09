/*
Data that should already be in DB
-- Add some basic information to make sure it works (no foreign keys)
start transaction;

INSERT INTO Users (Name, Rights) VALUES ("Craig", "Customer");
INSERT INTO Users (Name, Rights) VALUES ("Richard", "Admin");

INSERT INTO Locks (Name, BelongsTo, lockKey) VALUES ("Lock1", "Walmart");
INSERT INTO Locks (Name, BelongsTo, lockKey) VALUES ("Lock2", "Dymon");

commit; -- Ends the transaction
*/



-- Add some basic information to make sure it works (no foreign keys)
start transaction;

INSERT INTO Users (Name, Rights) VALUES ("Craig", "Admin");
INSERT INTO Users (Name, Rights) VALUES ("Richard", "Customer");
INSERT INTO Users (Name, Rights) VALUES ("Jess", "Admin");
INSERT INTO Users (Name, Rights) VALUES ("Jess", "Customer");

INSERT INTO Locks (Name, BelongsTo) VALUES ("Lock3", "Walmart");
INSERT INTO Locks (Name, BelongsTo) VALUES ("Lock4", "Dymon");

INSERT INTO Identities (User_Id, Name) VALUES ("1","CraigBasic");
INSERT INTO Identities (User_Id, Name) VALUES ("1","CraigSecond");
INSERT INTO Identities (User_Id, Name) VALUES ("2","RichardBasic");
INSERT INTO Identities (User_Id, Name) VALUES ("2","RichardSecond");

INSERT INTO Authenticators (AuthType, AuthKey, AuthSalt) VALUES ("Password","Bob","SaltySalt");
INSERT INTO Authenticators (AuthType, AuthKey, AuthSalt) VALUES ("Password","Blah","SaltySalt2");

INSERT INTO IdentityToAuth (Auth_Id, Identity_Id) VALUES ("1","1");
INSERT INTO IdentityToAuth (Auth_Id, Identity_Id) VALUES ("2","3");

INSERT INTO Registrations (Identity_Id, Lock_Id, ExpirationTime) VALUES ("1","1","2038-01-19 03:14:07");
INSERT INTO Registrations (Identity_Id, Lock_Id, ExpirationTime) VALUES ("3","2","2038-01-19 03:14:07");


commit; -- Ends the transaction