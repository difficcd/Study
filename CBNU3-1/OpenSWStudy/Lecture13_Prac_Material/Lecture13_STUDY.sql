

CREATE DATABASE databasename

CREATE TABLE relation_name (
attribute1 datatype  constraint,
attribute2 datatype  constraint,
attribute3 datatype  constraint,
...
);

-- INT, VARCHAR(n), DATE, FLOAT [50p]


-- SQL Syntax: insert
INSERT INTO relation_name (col1, col2, col3)
VALUES (val1, val2, val3)

-- SELECT syntax : order를 바꿀 순 없음.
SELECT [DISTINCT] attribute(s) -- (1) REQUIRED
FROM   relation(s)             -- (2) REQUIRED
[WHERE     condition]         -- (3) optional: filter rows   
[GROUP BY  attribute(s)]      -- (4) optional: aggregate groups
[HAVING    condition]         -- (5) optional: filter groups    
[ORDER BY  attribute(s)]      -- (6) optional: sort results
[LIMIT     number];           -- (7) optional: limit row count

-- SQL Syntax: DELETE
DELETE FROM relation_name WHERE condition


-- SQL Syntax: UPDATE, SET, WHERE
UPDATE relation_name
SET    attribute1 = value1, attribute2 = value2, ...
WHERE  condition