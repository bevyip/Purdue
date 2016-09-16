/**
 * Created by Shreyaansh on 9/13/2016.
 */
import org.jsoup.*;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.io.IOException;
import java.util.*;

public class WebCrawler {

    static HashMap storedLinks = new HashMap<String, Integer>();
    static Queue queue = new LinkedList<String>();
    static int counter = 0;
    static int dupCounter = 0;
    static int currentDepth = 0;

    public static void crawlBFS(String url) {
        try {
            Document doc = Jsoup.connect(url).get();

            Elements links = doc.select("a[href]");

            for (Element link: links) {
                String absUrl = link.attr("abs:href").toString();
                if (!storedLinks.containsKey(absUrl)) {
                    //System.out.println(absUrl + " " + counter);
                    storedLinks.put(absUrl, (int) storedLinks.get(url) + 1);
                    queue.add(absUrl);
                } else {
                    dupCounter++;
                }
            }
            //currentDepth++;
        } catch (Exception e) {
            //Do nothing
        }
    }
    public static void crawlDFS(String url) {
        try {
            
        } catch (Exception e) {
            //Do nothing
        }
    }
    public static void main(String[] args) {
        String url = "https://www.purdue.edu/";
        storedLinks.put(url, currentDepth++);
        queue.add(url);

        while(!queue.isEmpty() && counter <= 1000) {
            String u = (String) queue.remove();
            System.out.println("Current depth is " + storedLinks.get(u));
            crawlBFS(u);
            counter++;
        }

        System.out.println("The currentDepth is: " + currentDepth);
        System.out.println("The remaining number of elements are: " + queue.size());
        System.out.println("The number of duplicate urls encountered are: " + dupCounter);
    }
}
