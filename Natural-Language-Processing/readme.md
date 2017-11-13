# Natural Language Processing
The purpose of this project was to create a simple text based search engine with tf-idf ranking and content classification. The topics of this classification would be retrieved by the 20 news groups text [dataset](http://scikit-learn.org/stable/datasets/twenty_newsgroups.html#newsgroups), so two spiders were developed to crawl `CNN` and `TechCrunch` news websites, for different article posts. 

## Design 
### Crawler subsystem
This implementation consists of a [Scrapy](https://scrapy.org/) based Crawler. The crawler uses a MySQL pipeline, in order to store all the articles in a database. The table schema used, can be found bellow:

```sql
CREATE TABLE `website` (
  `docid` int(11) NOT NULL AUTO_INCREMENT,
  `guid` char(32) NOT NULL,
  `name` text,
  `content` text,
  `url` text,
  `updated` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
   PRIMARY KEY(`docid`),
   KEY(`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

Each post consists of a `docid` field, which is an auto increment integer used as a unique identifier of each document-post. The field `guid` is an MD5 key hash of the post's url, making it easy to store possible future updates. The `name`, `content`, `url` fields store the title, text body and link of the post respectively. The `updated` field, is updated whenever the crawler makes changes to a post instance.

Before firing up Scrapy, you should set your database configurations inside [db config](./GlwssikhCrawler/GlwssikhCrawler/db_config.py), and creating the table `website`. To run an  available spider, type the following commands inside the project's root directory:

```bash
$ cd GlwssikhCrawler
# Crawl the TechCrunch website
$ scrapy crawl tech
# Crawl CNN website
$ scrapy crawl cnn
```

The spiders should start automatically, and the downloaded content should be printed on your terminal.

### Indexer subsystem
Since the database is populated, we could start the Indexer to create the inverted index of the whole database. The inverted file is stored as a xml file, making it easy to be exported and imported for later use. An example of its format can be shown bellow:
```xml
<inverted_index>
<lemma name=”market”>
    <document id=”2” weight=”0.4”/>
    <document id=”50” weight=”0.34”/>
</lemma>
<lemma name=”apple”>
    <document id=”1234” weight=”0.65”/>
    <document id=”8” weight=”0.87”/>
    <document id=”36” weight=”0.45”/>
</lemma>
</inverted_index>
```
Starting up the Indexer, would retrieve all documents from database, utilizing the `DatabaseHandler` helper class, which would return all database contents striped from html tags in the posts' contents. The created inverted file, by default is saved as `invIdx.xml`. To execute the indexer, run the following command in the project's root directory:

```bash
$ python InvertedFile.py
```

### Query subsystem
The Query subsystem utilizes the exported inverted index, to perform queries on it. When run as a script, some test benchmarks are run uppon the inverted index, and then the input prompt appears. 

A better way to make queries is through the `webserver` script. The last launches a simple http server, running the Query module. The queries are inserted into a text field and the results are returned on descending order by their weight. To execute the web server, run the following command in the project's root directory:

``` bash
# By default the webserver launches on port 8000 
$ python webserver.py
```

### Classification subsystem

As mentioned above, the classifier uses the 20 newsgroups text topics, to classify the retrieved articles in the database. From this dataset a total of `8000` features are used for the vectorizer instance. The similarity of the categories are calculated with a cosine similarity, and the best topic matches are displayed for each an every article.  To execute the web server, run the following command in the project's root directory:
``` bash
# Classify max 50 posts (-1 for all)
$ python Classification.py 50
```

## Dependencies
Executing this project, requires:
* MySQL server ([MySQLdb connector](http://mysql-python.sourceforge.net/MySQLdb.html#mysqldb))
* Scrapy (https://scrapy.org/)
* NLTK (with Corpus + PorterStemmer) (http://www.nltk.org/)
* Scikit-learn (http://scikit-learn.org/)
