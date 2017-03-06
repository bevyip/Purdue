import	java.io.File;
import	java.util.*;

import gnu.trove.iterator.TObjectIntIterator;
import org.lemurproject.galago.core.index.Index;
import	org.lemurproject.galago.core.index.IndexPartReader;
import	org.lemurproject.galago.core.index.KeyIterator;
import	org.lemurproject.galago.core.index.LengthsReader;
import	org.lemurproject.galago.core.index.disk.DiskIndex;
import	org.lemurproject.galago.core.index.disk.DiskLengthsReader;
import	org.lemurproject.galago.core.index.stats.FieldStatistics;
import	org.lemurproject.galago.core.index.stats.NodeStatistics;
import	org.lemurproject.galago.core.parse.Document;
import	org.lemurproject.galago.core.parse.Tag;
import	org.lemurproject.galago.core.retrieval.Retrieval;
import	org.lemurproject.galago.core.retrieval.RetrievalFactory;
import	org.lemurproject.galago.core.retrieval.iterator.CountIterator;
import	org.lemurproject.galago.core.retrieval.iterator.LengthsIterator;
import	org.lemurproject.galago.core.retrieval.processing.ScoringContext;
import	org.lemurproject.galago.core.retrieval.query.Node;
import	org.lemurproject.galago.core.retrieval.query.StructuredQuery;
import org.lemurproject.galago.utility.ByteUtil;
import	org.lemurproject.galago.utility.Parameters;
import sun.jvm.hotspot.HelloWorld;

/**
 * Created by shreyaanshbassi on 11/9/16.
 */

public class Assignment3473 {

    static long documentCount = 0;

    public static void extractDocStatistics(String indexPath) {
        try {
            Retrieval retrieval = RetrievalFactory.instance(indexPath, Parameters.create());

            Node n = new Node();

            n.setOperator("lengths");
            n.getNodeParameters().set("part", "lengths");

            FieldStatistics stat = retrieval.getCollectionStatistics(n);

            double avgLength = stat.avgLength;
            documentCount = stat.documentCount;

            System.out.printf ("Average length   : %f \n", avgLength);
            System.out.printf ("Document Count   : %d \n", documentCount);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void getNumOfUniqueWords(String indexPath) {
        try {

            HashSet<String> vocabulary = new HashSet<String>();

            IndexPartReader reader = DiskIndex.openIndexPart(indexPath + "/postings");

            if (reader.getManifest().get("emptyIndexFile", false)) {
                System.out.println("EmptyIndexFile");
            }

            KeyIterator iter = reader.getIterator();

            while (iter.nextKey()) {
                if (isAlpha(iter.getKeyString()))
                    vocabulary.add(iter.getKeyString());
            }
            reader.close();

            System.out.println("The number of unique words are: " + vocabulary.size());

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void extractLongestDocStatistics(String indexPath) {
        try {
            Retrieval retrieval = RetrievalFactory.instance(indexPath, Parameters.create());

            Node n = new Node();

            n.setOperator("lengths");
            n.getNodeParameters().set("part", "lengths");

            FieldStatistics stat = retrieval.getCollectionStatistics(n);

            long maxLength = stat.maxLength;

            LengthsReader lengthsReader = new DiskLengthsReader(indexPath + "/lengths");

            LengthsIterator lengthsIterator = (LengthsIterator) lengthsReader.getLengthsIterator();
            ScoringContext scoringContext = new ScoringContext();

            ArrayList<Long> list = new ArrayList<Long>();

            while (!lengthsIterator.isDone()) {
                scoringContext.document = lengthsIterator.currentCandidate();

                if (lengthsIterator.length(scoringContext) == maxLength)
                    list.add(scoringContext.document);

                lengthsIterator.movePast(lengthsIterator.currentCandidate());
            }

            System.out.println("The maximum length is: " + maxLength);
            System.out.println("The document ids' are: " + list.toString());

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void getCount(String pathIndex, String[] terms) {
        try {
            Retrieval r = RetrievalFactory.instance(pathIndex, Parameters.create());

            for (String term: terms) {
                String query = term;
                Node node = StructuredQuery.parse(query);
                node.getNodeParameters().set("queryType", "count");
                node = r.transformQuery(node, Parameters.create());

                NodeStatistics stat = r.getNodeStatistics(node);
                long maxCount = stat.maximumCount;
                long nodeDocCount = stat.nodeDocumentCount;
                long nodeFrequency = stat.nodeFrequency;

                System.out.println("\n\n Term: " + term);
                System.out.println("Maximum Count       : " + maxCount);
                System.out.println("Node Document Count : " + nodeDocCount);
                System.out.println("Node Freq           : " + nodeFrequency + "\n");
            }
        } catch (Exception e) {

        }
    }

    public static void getPostings(String indexPath, String term) {
        try {
            String field = "text";
            File pathPosting = new File( new File( indexPath ), "postings" );

            DiskIndex index = new DiskIndex( indexPath );
            IndexPartReader posting = DiskIndex.openIndexPart( pathPosting.getAbsolutePath() );

            System.out.printf( "%-10s%-15s%-10s\n", "DOCID", "DOCNO", "FREQ" );

            KeyIterator vocabulary = posting.getIterator();

            if ( vocabulary.skipToKey( ByteUtil.fromString( term ) ) && term.equals( vocabulary.getKeyString() ) ) {

                CountIterator iterator = (CountIterator) vocabulary.getValueIterator();
                ScoringContext sc = new ScoringContext();
                while ( !iterator.isDone() ) {
                    sc.document = iterator.currentCandidate();
                    int freq = iterator.count( sc ); // get the frequency of the term in the current document
                    String docno = index.getName( sc.document ); // get the docno (external ID) of the current document
                    System.out.printf( "%-10s%-15s%-10s\n", sc.document, docno, freq );
                    iterator.movePast( iterator.currentCandidate() ); // jump to the entry right after the current one
                }
            }

            posting.close();
            index.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static Boolean isAlpha(String string) {
        return string.matches("[a-zA-Z]+");
    }

    public static void main(String[] args) {
        String indexPath = "/Users/shreyaanshbassi/Documents/Development/galago-3.10/core/target/appassembler/bin/none";

        // Part (a)
        extractDocStatistics(indexPath);

        // Part (b)
        getNumOfUniqueWords(indexPath);

        // Part (c)
        extractLongestDocStatistics(indexPath);

        // Part (d)
        String[] terms = {"maximum", "entropy", "data", "science"};
        getCount(indexPath, terms);

        // Part (e) and (f)
            // Calculated by calculator and also part (g) code

        // Part (g)
        getPostings(indexPath, "maximum");
        getPostings(indexPath, "entropy");
    }
}
