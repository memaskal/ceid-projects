#!/usr/bin/python

import nltk
from nltk.stem.porter import PorterStemmer
from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity
from DatabaseHandler import DatabaseHandler


class Classification:

	MAX_FEATURES = 8000
	stemmer = PorterStemmer()

	tfidf = None
	data_set = None

	"""
		Performs stemming using a porter
		stemmer instance on a token list
	"""
	def stem_tokens(self, tokens, stemmer):
		stemmed = []
		for item in tokens:
			stemmed.append(stemmer.stem(item))
		return stemmed

	"""
		Split text to tokens and returns
		a stem list
	"""
	def tokenize(self, text):
		tokens = nltk.word_tokenize(text)
		stems = self.stem_tokens(tokens, self.stemmer)
		return stems

	"""
		Loads the 20 news group categories and
		creates a vectorizer instance for the whole
		data set
	"""
	def fit_dataset(self):
		# Load data set
		self.data_set = fetch_20newsgroups(subset='test',
						remove=('headers', 'footers', 'quotes'))

		# Tfidf Vectorizer uses Max features = 8000
		self.tfidf = TfidfVectorizer(tokenizer=self.tokenize,
									 stop_words='english',
									 max_features=self.MAX_FEATURES,
									 decode_error='ignore')

		# Create all the vectors for the 20 news group set
		return self.tfidf.fit_transform(self.data_set.data)

	"""
		Returns the category name of a file in the data set
		given its index number in the set
	"""
	def get_filename(self, idx):
		return self.data_set.filenames[idx].split("/")[-2]

	"""
		Returns a vector representation of any
		text input according to the data set
	"""
	def vectorize_text(self, text):
		return self.tfidf.transform([text])

	def main(self, MAX_POSTS = -1):
		# Load and fit data set
		print "Fitting data set"
		tfs = self.fit_dataset()

		# Take MAX_POSTS posts to classify from database
		posts = DatabaseHandler().get_posts(MAX_POSTS).values()
		for post in posts:
			# Creating vector for new document
			vec = self.vectorize_text(post.text)

			# Calculate cosine similarity
			sim = cosine_similarity(tfs, vec).flatten()
			
			# Find top matching category
			best_match_doc = sim.argmax()
			print "\nUrl : %s matches best:" % post.url
			print "Category %s with %.5f sim" % (self.get_filename(best_match_doc), sim[best_match_doc])


if __name__ == "__main__":
	import sys
	count = -1
	if len(sys.argv) > 1:
		count = int(sys.argv[1])
	# run main
	Classification().main(count)
