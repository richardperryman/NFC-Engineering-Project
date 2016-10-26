
-- Create all of our tables
CREATE TABLE Users(
	User_Id int NOT NULL auto_increment,
	Name varchar(255) NOT NULL,
	Rights varchar(255) NOT NULL,
	PRIMARY KEY (User_Id)
);

CREATE TABLE Identities(
	Identity_Id int NOT NULL auto_increment,
	User_Id int,
	Name varchar(255) NOT NULL,
	PRIMARY KEY (Identity_Id),
	FOREIGN KEY (User_Id) REFERENCES Users(User_Id)
);

CREATE TABLE Authenticators(
	Auth_Id int NOT NULL auto_increment,
	Identity_Id int,
	AuthType varchar(255) NOT NULL,
	AuthKey varchar(255) NOT NULL,
	PRIMARY KEY (Auth_Id),
	FOREIGN KEY (Identity_Id) REFERENCES Identities(Identity_Id)
);

CREATE TABLE Locks(
	Lock_Id int NOT NULL auto_increment,
	Name varchar(255) NOT NULL,
	BelongsTo varchar(255) NOT NULL,
	lockKey varchar(255) NOT NULL,
	PRIMARY KEY (Lock_Id)
);

CREATE TABLE Registrations(
	Reg_Id int NOT NULL auto_increment,
	Identity_Id int,
	Lock_Id int,
	PRIMARY KEY (Reg_Id),
	FOREIGN KEY (Identity_Id) REFERENCES Identities(Identity_Id),
	FOREIGN KEY (Lock_Id) REFERENCES Locks(Lock_Id)
);


-- Add some basic information to make sure it works (no foreign keys)
start transaction;

INSERT INTO Users (Name, Rights) VALUES ("Craig", "Customer");
INSERT INTO Users (Name, Rights) VALUES ("Richard", "Admin");

INSERT INTO Locks (Name, BelongsTo, lockKey) VALUES ("Lock1", "Walmart", "QfEUdIYqjc");
INSERT INTO Locks (Name, BelongsTo, lockKey) VALUES ("Lock2", "Dymon", "BoIiYdEsZE");

commit; -- Ends the transaction