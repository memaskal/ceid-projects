#!/usr/bin/python

import nltk
from nltk.corpus import stopwords
from math import log
from DatabaseHandler import DatabaseHandler
import xml.etree.ElementTree as ET

class InvertedFile:

	# Default filename
	INDEX_FILENAME = "invIdx.xml"

	# Closed tag categories set
	CLOSED_TAGS = {'CD', 'CC', 'DT', 'EX', 'IN',
				   'LS', 'MD', 'PDT', 'POS', 'PRP',
				   'PRP$', 'RP', 'TO', 'UH', 'WDT',
				   'WP', 'WP$', 'WRB'}

	word_docs = {}
	inverted_index = {}

	"""
		Computes the full inverted index of the
		tokens in word_docs dictionary
	"""
	def compute_inverted_index(self, total_docs):
		word_count, total = self.count_words()
		# for each document
		for doc_id in word_count:
			# for each term in document
			dict = word_count[doc_id]
			for word in dict:
				# TF(t) = (Number of times term t appears in a document) / (Total number of terms in the document).
				tf = float(dict[word]) / total[doc_id]
				if word not in self.inverted_index:
					self.inverted_index[word] = {}
				# update weight partially
				self.inverted_index[word][doc_id] = tf

		# IDF(t) = log_e(Total number of documents / Number of documents with term t in it).
		for word in self.word_docs:
			idf = log(float(total_docs) / len(self.word_docs[word]['docs']))
			for doc_id in self.inverted_index[word]:
				self.inverted_index[word][doc_id] *= idf

	"""
		Returns a dictionary of document ids containing
		their terms frequency and a total dictionary 
		containing its document's total word length
	"""
	def count_words(self):
		word_count = {}
		total = {}
		for word in self.word_docs:
			for ids in self.word_docs[word]['docs']:
				if ids not in word_count:
					word_count[ids] = {}
					total[ids] = 0
				if word not in word_count[ids]:
					word_count[ids][word] = 1
				else:
					word_count[ids][word] += 1
				total[ids] += 1
		return word_count, total

	"""
		Given a string and a document id, creates
		a file containing words with their PosTag
		and the documents they appear
	"""
	def pos_tagger(self, str, id):
		stops = set(stopwords.words("english"))
		str = str.decode('unicode_escape').encode('ascii', 'ignore').lower()
		tokens = nltk.word_tokenize(str)
		tags = nltk.pos_tag(tokens)

		for tag in tags:
			word, categ = tag
			if (categ in self.CLOSED_TAGS or	# tag is in closed words categories
							word in stops or	# word in nltk's stop words
							categ == word):		# word is special char !,?#:
				continue
			if word not in self.word_docs:
				# insert new word to dictionary
				self.word_docs[word] = {'docs': [id]}
			elif id not in self.word_docs[word]['docs']:
				# update the document ids if they are unique
				self.word_docs[word]['docs'].append(id)

	"""
		Loads all posts from database
		and creates the inverted index
	"""
	def create_index(self):
		self.word_docs = {}
		self.inverted_index = {}

		db = DatabaseHandler()
		posts = db.get_posts()
		total_posts = len(posts)
		
		for doc_id in posts:
			self.pos_tagger(posts[doc_id].text, doc_id)

		self.compute_inverted_index(total_posts)

	"""
		Export inverted file as xml
	"""
	def exportXML(self, filename=INDEX_FILENAME):
		root = ET.Element('inverted_index')
		for word in self.inverted_index:
			lemma = ET.SubElement(root, 'lemma', {'name': word})
			docs = self.inverted_index[word]
			for doc_id in docs:
				sdocid = "%d" % doc_id
				tfidf = "%.2f" % docs[doc_id]
				ET.SubElement(lemma, 'document', {'id': sdocid, 'weight': tfidf})
		ET.ElementTree(root).write(filename)

	"""
		Import inverted file from xml
	"""
	def importXML(self, filename=INDEX_FILENAME):
		self.inverted_index = {}
		tree = ET.ElementTree()
		tree.parse(filename)

		for lemma in list(tree.iter("lemma")):
			word = lemma.attrib['name']
			self.inverted_index[word] = {}
			for doc in list(lemma.iter("document")):
				doc_id = doc.attrib['id']
				tfidf = float(doc.attrib['weight'])
				self.inverted_index[word][doc_id] = tfidf


if __name__ == "__main__":
	invf = InvertedFile()
	print "Calculating"
	invf.create_index()
	print "Exporting"
	invf.exportXML()
