-- List the names of all people who starred in a movie in which Kevin Bacon also starred
SELECT name
FROM people
    JOIN stars s1 ON s1.movie_id = s2.movie_id
    JOIN stars s2 ON s2.person_id = people.id
WHERE s1.person_id = (
        SELECT id
        FROM people
        WHERE name='Kevin Bacon'
    )
    AND name <> 'Kevin Bacon';