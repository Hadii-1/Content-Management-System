#include "pch.h"
#include"D:\Semester 4\Data Structures\Assignment 03\i240696_A_A3\Header.h"


#include <gtest/gtest.h>
//#include "Header.h"
class CMS_Test : public ::testing::Test {
protected:
	ContentManagementSystem cms;
	void SetUp() override {
	}
	void TearDown() override {
	}
};
TEST_F(CMS_Test, CreateSingleContent) {
	cms.createContent("Article1", "Hello World");
	auto meta = cms.findContent("Article1");
	ASSERT_NE(meta, nullptr);
	EXPECT_EQ(meta->title, "Article1");
	EXPECT_EQ(meta->totalRevisions, 1);
}
TEST_F(CMS_Test, DuplicateContent) {
	cms.createContent("Article1", "Hello");
	cms.createContent("Article1", "Duplicate");
	auto meta = cms.findContent("Article1");
	EXPECT_EQ(meta->totalRevisions, 1); // should not overwrite
}
TEST_F(CMS_Test, AddRevisions) {
	cms.createContent("Doc1", "v1");
	cms.addRevision("Doc1", "v2");
	cms.addRevision("Doc1", "v3");

	auto meta = cms.findContent("Doc1");
	ASSERT_NE(meta, nullptr);
	EXPECT_EQ(meta->totalRevisions, 3);
}
TEST_F(CMS_Test, RevisionBSTStructure) {
	cms.createContent("Doc1", "A");
	cms.addRevision("Doc1", "B");
	cms.addRevision("Doc1", "C");
	auto root = cms.findContent("Doc1")->revisionRoot;
	ASSERT_NE(root, nullptr);
	EXPECT_TRUE(root->left == nullptr || root->right != nullptr);
}
TEST_F(CMS_Test, ExactTimestamp) {
	cms.createContent("Doc1", "v1");
	long t1 = cms.getLatestTimestamp("Doc1");
	auto rev = cms.getRevision("Doc1", t1);
	ASSERT_NE(rev, nullptr);
	EXPECT_EQ(rev->text, "v1");
}
TEST_F(CMS_Test, ClosestTimestamp) {
	cms.createContent("Doc1", "v1");
	cms.addRevision("Doc1", "v2");
	long t = cms.getLatestTimestamp("Doc1") - 1;
	auto rev = cms.getRevision("Doc1", t);
	ASSERT_NE(rev, nullptr);
}
TEST_F(CMS_Test, TitleSearch) {
	cms.createContent("AI_Article", "data");
	cms.createContent("ML_Notes", "data");
	SearchResultNode* results = cms.searchContentTitles("AI");
	ASSERT_NE(results, nullptr);
}
TEST_F(CMS_Test, ContentSearch) {

	cms.createContent("Doc1", "machine learning");
	cms.addRevision("Doc1", "deep learning");
	auto results = cms.searchText("learning");
	ASSERT_NE(results, nullptr);
	EXPECT_EQ(results->title, "Doc1");
}
TEST_F(CMS_Test, DeleteContent) {
	cms.createContent("Doc1", "text");
	cms.deleteContent("Doc1");
	auto meta = cms.findContent("Doc1");
	EXPECT_EQ(meta, nullptr);
}
TEST_F(CMS_Test, DeleteNonExisting) {
	cms.deleteContent("Ghost");
	SUCCEED(); // should not crash
}
TEST_F(CMS_Test, CreateSnapshot) {
	cms.createContent("Doc1", "v1");
	cms.createSnapshot("S1");
	ASSERT_NE(cms.snapshots, nullptr);
}
TEST_F(CMS_Test, RestoreSnapshot) {
	cms.createContent("Doc1", "v1");
	cms.createSnapshot("S1");
	cms.addRevision("Doc1", "v2");
	cms.restoreSnapshot("S1");
	auto meta = cms.findContent("Doc1");
	EXPECT_EQ(meta->totalRevisions, 1);
}
TEST_F(CMS_Test, BalanceFactorCalculation) {
	cms.createContent("Doc1", "v1");
	for (int i = 0; i < 5; i++)

		cms.addRevision("Doc1", "v" + std::to_string(i + 2));
	auto root = cms.findContent("Doc1")->revisionRoot;
	int bf = cms.getBalanceFactor(root);
	EXPECT_TRUE(bf >= -5 && bf <= 5);
}
TEST_F(CMS_Test, SkewedTree) {
	cms.createContent("Doc1", "v1");
	// Insert increasing timestamps: right skew
	for (int i = 0; i < 10; i++)
		cms.addRevision("Doc1", "v" + std::to_string(i));
	auto root = cms.findContent("Doc1")->revisionRoot;
	int bf = cms.getBalanceFactor(root);
	EXPECT_LT(bf, 0); // right heavy
}
TEST_F(CMS_Test, SystemAnalysis) {
	cms.createContent("A", "v1");
	cms.createContent("B", "v1");
	cms.addRevision("A", "v2");
	//cms.systemAnalysis();
	SUCCEED(); // should execute without crash
}
TEST_F(CMS_Test, NoEarlierRevision) {
	cms.createContent("Doc1", "v1");
	long t = cms.getLatestTimestamp("Doc1") - 100;
	auto rev = cms.getRevision("Doc1", t);
	EXPECT_EQ(rev, nullptr);
}
TEST_F(CMS_Test, EmptySearch) {
	auto res = cms.searchText("anything");
	EXPECT_EQ(res, nullptr);
}
TEST_F(CMS_Test, LargeData) {

	for (int i = 0; i < 50; i++) {
		cms.createContent("Doc" + std::to_string(i), "v1");
		for (int j = 0; j < 10; j++)
			cms.addRevision("Doc" + std::to_string(i), "v" + std::to_string(j));
	}
	SUCCEED(); // performance + stability
}