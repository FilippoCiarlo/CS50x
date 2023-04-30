--  List the names of the songs that feature other artists
SELECT name
FROM songs
WHERE name LIKE '%feat%';