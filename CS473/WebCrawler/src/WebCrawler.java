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

    static Queue q = new LinkedList();
    static Stack s = new Stack();
    static Hashtable<String, String> links = new Hashtable<>();
    static int counter = 0;

    public static void crawlBFS(String url) throws IOException {

        try {
            //System.out.println(url);
            Document doc = Jsoup.connect(url).get();

            String title = doc.title();

            links.put(url, title);

            //Extract the URLs from the page here
            Elements link = doc.select("a[href]");

            for (Element linc:link) {
                String u = linc.attr("abs:href").toString();
                if (!links.containsKey(u)) {
                    System.out.println(u + " " + counter);
                    if (!q.contains(u)) {
                        q.add(u);
                    }
                    links.put(u, linc.toString());
                }
            }

        } catch (Exception e) {
        }
    }

    public static void crawlDFS(String url) throws IOException {
        try {
            Document doc = Jsoup.connect(url).get();
            String title = doc.title();

            links.put(url, title);

            Elements elem = doc.select("a[href]");

            for (Element linc : elem) {
                String u = linc.attr("abs:href").toString();
                if (!links.containsKey(u)) {
                    crawlDFS(u);
                }
            }
        } catch (Exception e) {
            //Do nothing have fun
        }
    }

    public static void main(String[] args) throws IOException {
        String url = "http://www.purdue.edu/";

        q.add(url);

        while (counter != 100) {
            counter++;
            String ur = (String) q.remove();
            crawlBFS(ur);
        }

        System.out.println(q.size());
        System.out.println("This is what remains of the queue!");

        counter = 0;
        while (counter != 100) {
            counter++;
            String ur = (String) s.pop();
            crawlDFS(ur);
        }

        System.out.println(s.size());
        System.out.println("This is what remains of the stack!");
    }
}