-- Determine the number of movies with an IMDb rating of 10.0
SELECT COUNT(*)
FROM movies
    JOIN ratings ON movies.id=ratings.movie_id
WHERE rating=10.0;