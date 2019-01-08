package edu.gatech.cse6242;
import java.util.StringTokenizer;
import java.lang.Object;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;
import org.apache.hadoop.util.*;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import java.io.IOException;

public class Q4 {

  public static class DegreeDiffMapper
	    extends Mapper<Object, Text, IntWritable, IntWritable>{

	  private IntWritable node = new IntWritable();
	  private IntWritable out_degree = new IntWritable(1);
	  private IntWritable in_degree = new IntWritable(-1);

	  public void map(Object key, Text value, Context context
	                ) throws IOException, InterruptedException {
	  StringTokenizer itr = new StringTokenizer(value.toString(),  "\n");
	  while (itr.hasMoreTokens()) {
	  	
	  	String next_itr = itr.nextToken();
	  	String next_split[] = next_itr.split("\t");

        node.set(Integer.parseInt(next_split[0]));
        context.write(node, out_degree);

        node.set(Integer.parseInt(next_split[1]));
        context.write(node, in_degree);

	  }
	}
  }

  public static class NodeCountMapper
	    extends Mapper<Object, Text, IntWritable, IntWritable>{

	  private IntWritable diff = new IntWritable();
	  private IntWritable cnt = new IntWritable(1);

	  public void map(Object key, Text value, Context context
	                ) throws IOException, InterruptedException {
	  StringTokenizer itr = new StringTokenizer(value.toString(),  "\n");
	  while (itr.hasMoreTokens()) {
	  	
	  	String next_itr = itr.nextToken();
	  	String next_split[] = next_itr.split("\t");

        diff.set(Integer.parseInt(next_split[1]));
        context.write(diff, cnt);
	  }
	}
  }

  public static class IntSumReducer
       extends Reducer<IntWritable,IntWritable,IntWritable,IntWritable> {
    
    private IntWritable result = new IntWritable();
    public void reduce(IntWritable key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }
      result.set(sum);
      context.write(key, result);
    }
  }


  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    /* TODO: Needs to be implemented */

    //finding the difference between out-degree and in-degree for each node
    Job job_1 = Job.getInstance(conf, "Q4_1");
    //calculating the node count of each degree difference
    Job job_2 = Job.getInstance(conf, "Q4_2");

    job_1.setJarByClass(Q4.class);
    job_1.setMapperClass(DegreeDiffMapper.class);
    job_1.setCombinerClass(IntSumReducer.class);
    job_1.setReducerClass(IntSumReducer.class);
    job_1.setOutputKeyClass(IntWritable.class);
    job_1.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job_1, new Path(args[0]));
    FileOutputFormat.setOutputPath(job_1, new Path("job_1_output"));
    job_1.waitForCompletion(true);

    job_2.setJarByClass(Q4.class);
    job_2.setMapperClass(NodeCountMapper.class);
    job_2.setCombinerClass(IntSumReducer.class);
    job_2.setReducerClass(IntSumReducer.class);
    job_2.setOutputKeyClass(IntWritable.class);
    job_2.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job_2, new Path("job_1_output"));
    FileOutputFormat.setOutputPath(job_2, new Path(args[1]));
    System.exit(job_2.waitForCompletion(true) ? 0 : 1);
  }
}
