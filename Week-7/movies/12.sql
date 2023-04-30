-- List the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred
SELECT title
FROM movies
    JOIN stars s1 ON s1.movie_id=movies.id
    JOIN stars s2 ON s2.movie_id=movies.id
    JOIN people p1 ON p1.id=s1.person_id
    JOIN people p2 ON p2.id=s2.person_id
WHERE p1.name='Johnny Depp'
    AND p2.name='Helena Bonham Carter';