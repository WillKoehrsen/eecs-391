# EECS 391 HW5
# 
# 1. B.
library(tidyverse)
library(ggthemes)

# Define possible values of y
y <- seq(0, 4, 1)

# Function that takes in a value of y and returns the probability of y
# given n = 4 and theta = 0.74
prob <- function(y) {
  first_term <- (factorial(4) / (factorial((4 - y)) * factorial(y)))
  second_term <- (0.75 ^ y) * (0.25 ^ (4 - y))
  return(first_term * second_term)
}


# The probability distribution of y
prob_y <- sapply(y, prob)

# Created y for smooth lines
y_smooth <- seq(0, 4, by = 4/100)
prob_y_smooth <- sapply(y_smooth, prob)

# Plot the likelihood of y under the assumption theta = 0.75 and n = 4
plot(y, prob_y, main = 'Likelihood of y') + lines(y_smooth, prob_y_smooth) + 
  text(y, prob_y - 0.04, labels = round(prob_y, 2))

# 1. C.
# Calculates posterior probability for theta given n and y
calc_posterior <- function(theta, y, n) {
  first_term <- factorial(n + 1) / (factorial(n - y) * factorial(y))
  second_term <- (theta ^ y) * ((1 - theta) ^ (n - y))
  return(first_term * second_term)
}

y <- c(1, 2, 2, 3)
n <- c(1, 2, 3, 4)
theta <- seq(0, 1, 1/1000)

posterior_df <- as.data.frame(matrix(nrow = length(theta) * 4, ncol = 4))
names(posterior_df) <- c('n', 'y', 'theta', 'posterior')

i <- 1
all_posteriors <- c()
for (head_count in y) {
  posteriors <- sapply(theta, calc_posterior, y = head_count, n = i)
  all_posteriors <- c(all_posteriors, posteriors)
  i <- i + 1
}

posterior_df$n <- c(rep(1, 1001), rep(2, 1001), 
                    rep(3, 1001), rep(4, 1001))
posterior_df$y <- c(rep(1, 1001), rep(2, 2002),
                    rep(3, 1001))

posterior_df$n_y <- c(rep(c('n = 1, y = 1'), 1001), rep(c('n = 2, y = 2'), 1001),
                      rep(c('n = 3, y = 2'), 1001), rep(c('n = 4, y = 3'), 1001))

posterior_df$theta <- c(rep(theta, 4))

posterior_df$posterior <- all_posteriors

ggplot(posterior_df, aes(theta, posterior)) + geom_point(col = 'darkgreen') + 
  geom_line(col = 'darkgreen') +
  facet_wrap(~n_y) + ylab('Posterior for theta') + 
ggtitle('Posterior Distributions for theta') + theme_solarized_2() + 
  theme(panel.grid = element_blank(), text =  element_text(face = 'bold', color = 'black'), 
        title = element_text(face = 'bold', color = 'black'))

# 2. A.
# 

# Prior probabilities and list of hypotheses expressed as probability of lime candy
priors <- list('h1' = 0.1, 'h2' = 0.2, 'h3' = 0.4, 'h4' = 0.2, 'h5' = 0.1)
hypotheses <- list('h1' = 0.0, 'h2' = 0.25, 'h3' = 0.5, 'h4' = 0.75, 'h5' = 1.0)

# The likelihood of a hypothesis (theta) given observations of successes, y, 
# and total number of trials
calc_likelihood <- function(theta, y, n) {
  first_term <- factorial(n) / (factorial(n - y) * factorial(y))
  second_term <- (theta ^ y) * ((1 - theta) ^ (n - y))
  return(first_term * second_term)
}

# Calculates the posterior for each of the five hypothesis under one true hypothesis
calc_posteriors <- function(true_hyp) {
  
  # Set seed to ensure consistent results
  set.seed(42)
  # Create a dataframe to hold results
  posts_df <- as.data.frame(matrix(ncol = 8))
  names(posts_df) <- c('n', 'y', 'h1', 'h2', 'h3', 'h4', 'h5', 'next_lime')
  
  # Vector of observations under the true hypothesis
  limes <- c(0, rbinom(n = 99, size = 1, prob = true_hyp))
  total_limes <- cumsum(limes)
  
  # Priors for each hypothesis and the conditional probability of a lime candy
  # for each hypothesis
  priors <- list('h1' = 0.1, 'h2' = 0.2, 'h3' = 0.4, 'h4' = 0.2, 'h5' = 0.1)
  lime_pct <- list('h1' = 0.0, 'h2' = 0.25, 'h3' = 0.5, 'h4' = 0.75, 'h5' = 1.0)
  
  # Iterate through the observations and calculate posteriors and 
  # chance of next lime
  for (i in seq(1, 100, 1)) {
    n_lime <- total_limes[i]
    
    # Calculate the unnormalized posterior for each hypothesis
    h1_post <- calc_likelihood(lime_pct$h1, n_lime, i - 1) * priors$h1
    h2_post <- calc_likelihood(lime_pct$h2, n_lime, i - 1) * priors$h2
    h3_post <- calc_likelihood(lime_pct$h3, n_lime, i - 1) * priors$h3
    h4_post <- calc_likelihood(lime_pct$h4, n_lime, i - 1) * priors$h4
    h5_post <- calc_likelihood(lime_pct$h5, n_lime, i - 1) * priors$h5
    
    # Calculate the normalization constant and normalize the posteriors
    posts <- c(h1_post, h2_post, h3_post, h4_post, h5_post)
    
    norm_constant <- sum(posts)
    norm_posts <- posts / norm_constant
    
    # The probability of a lime next
    next_lime <- sum(unlist(lime_pct) * norm_posts)
    
    # Explicitly add results to a dataframe
    posts_df <- add_row(posts_df, n = i - 1, y = n_lime, h1 = norm_posts[1], h2 = norm_posts[2], 
                        h3 = norm_posts[3], h4 = norm_posts[4], h5 = norm_posts[5],
                        next_lime = next_lime)
    
  }
  
  return(posts_df[-1, ])
}

# Assume each hypothesis is true and calculate the corresponding distributions
h1_true <- calc_posteriors(0.0)
h2_true <- calc_posteriors(0.25)
h3_true <- calc_posteriors(0.5)
h4_true <- calc_posteriors(0.75)
h5_true <- calc_posteriors(1.0)

# Function to graph posterior distributions for each hypothesis and the 
# probability the next candy is a lime
# Takes in a dataframe with posteriors and the string of the true hypothesis
# that was used to calculate the posteriors
graph_posts <- function(post_df, true_hyp_str) {
  # Titles for figures
  main1 <- sprintf('Posteriors under %s', true_hyp_str)
  main2 <- sprintf('Probability of next lime under %s', true_hyp_str)
  # Put results in long format with each row and observation and each column
  # a variable 
  long_post <- gather(post_df, key = 'hyp', value = 'post', -n, -y, -next_lime)
  color_vector <- c('blue', 'red', 'darkgreen', 'darkorange', 'black')
  
  th  <- theme(axis.text = element_text(color = 'black'))
  
   # Graph the posteriors for each hypothesis under the true hypothesis
  p1 <- ggplot(long_post, aes(n, post, col = hyp, group = hyp, shape = hyp)) + geom_line() + 
    geom_point() + xlab('Number of Observations') + ylab('Posterior') + 
    ggtitle(main1) + theme_economist(12) +  th + scale_color_manual(values = color_vector)
  
  # Graph the probability of the next candy being a lime
  p2 <- ggplot(post_df, aes(n, next_lime)) + geom_line(col = 'red') + geom_point(col = 'red') + 
    xlab('Number of Observations') + ylab('Probability') + ggtitle(main2) + 
    coord_cartesian(ylim = c(0, 1)) + scale_y_continuous(breaks = seq(0, 1, 0.1)) + 
    theme_economist(12) + th 
  
  # Display the graphs
  print(p1)
  print(p2)
}

# Graph results for each assumed hypothesis
graph_posts(h1_true, 'h1')
graph_posts(h2_true, 'h2')
graph_posts(h3_true, 'h3')
graph_posts(h4_true, 'h4')
graph_posts(h5_true, 'h5')


# 2. B.
# 
# 

# Find the minimum number of observations for a given confidence level
# Posterior df is generated from the specified true hypothesis (lime pct)
find_minimum_obs <- function(confidence_level, post_df, true_hyp_str) {
 
   # Filter dataframe to observations with true posterior greater than confidence level
  above_cl <- post_df[post_df[true_hyp_str] > confidence_level, ] 
  minimum_obs <- above_cl[[which.min(above_cl$n), 'n']]
  
  # Return the minimum observations for the specified confidence level
  return(minimum_obs)
}

# Find minimum observations required under each hypothesis
h1_min <- find_minimum_obs(0.9, h1_true, 'h1')
h2_min <- find_minimum_obs(0.9, h2_true, 'h2')
h3_min <- find_minimum_obs(0.9, h3_true, 'h3')
h4_min <- find_minimum_obs(0.9, h4_true, 'h4')
h5_min <- find_minimum_obs(0.9, h5_true, 'h5')

# Vectors for plotting results
hyps <- c(0.0, 0.25, 0.5, 0.75, 1.0)
min_obs <- c(h1_min, h2_min, h3_min, h4_min, h5_min)

# Plot the results
plot(x = hyps, y = min_obs, xaxt = 'n',  xlim = c(0, 1.1), 
     xlab = 'true hypothesis (percentage lime)', 
     ylab = 'observations', main = 'Minimum Observations for 90% Confidence') + 
  text(x = hyps + 0.04, y = min_obs, labels = min_obs) + axis(side = 1, at = hyps)

# 2. C.
# 
#

# Generate 100 random datasets of 100 observations and average
generate_random <- function() {
  observations <- c(rep(0, 100))
  priors <- c(0.1, 0.2, 0.4, 0.2, 0.1)
  i <- 1
  total <- 0
  for (prob in c(0.0, 0.25, 0.5, 0.75, 1.0)) {
    iterations <- priors[i]
    for (m in 1:(iterations * 100)) {
      observations <- (observations + rbinom(n = 100, size = 1, prob = prob))
      total <- total + 1
    }
    i <- i + 1
  }
  # Take mean and round to either a 0 or 1 
  observations <- (observations / total)
  observations[which(observations >= 0.5)] = 1L
  observations[which(observations < 0.5)]  = 0L
  return(observations)
}

# Generate the random dataset
random_obs <- generate_random()

# Calculates the posterior for each of the five hypothesis given a set of observations
calc_posteriors_given_obs <- function(obs) {
  

  # Create a dataframe to hold results
  posts_df <- as.data.frame(matrix(ncol = 8))
  names(posts_df) <- c('n', 'y', 'h1', 'h2', 'h3', 'h4', 'h5', 'next_lime')
  
  # Vector of observations provided
  limes <- c(0, obs)
  total_limes <- cumsum(limes)
  
  # Priors for each hypothesis and the conditional probability of a lime candy
  # for each hypothesis
  priors <- list('h1' = 0.1, 'h2' = 0.2, 'h3' = 0.4, 'h4' = 0.2, 'h5' = 0.1)
  lime_pct <- list('h1' = 0.0, 'h2' = 0.25, 'h3' = 0.5, 'h4' = 0.75, 'h5' = 1.0)
  
  # Iterate through the observations and calculate posteriors and 
  # chance of next lime
  for (i in seq(1, 101, 1)) {
    n_lime <- total_limes[i]
    
    # Calculate the unnormalized posterior for each hypothesis
    h1_post <- calc_likelihood(lime_pct$h1, n_lime, i - 1) * priors$h1
    h2_post <- calc_likelihood(lime_pct$h2, n_lime, i - 1) * priors$h2
    h3_post <- calc_likelihood(lime_pct$h3, n_lime, i - 1) * priors$h3
    h4_post <- calc_likelihood(lime_pct$h4, n_lime, i - 1) * priors$h4
    h5_post <- calc_likelihood(lime_pct$h5, n_lime, i - 1) * priors$h5
    
    # Calculate the normalization constant and normalize the posteriors
    posts <- c(h1_post, h2_post, h3_post, h4_post, h5_post)
    
    norm_constant <- sum(posts)
    norm_posts <- posts / norm_constant
    
    # The probability of a lime next
    next_lime <- sum(unlist(lime_pct) * norm_posts)
    
    # Explicitly add results to a dataframe
    posts_df <- add_row(posts_df, n = i - 1, y = n_lime, h1 = norm_posts[1], h2 = norm_posts[2], 
                        h3 = norm_posts[3], h4 = norm_posts[4], h5 = norm_posts[5],
                        next_lime = next_lime)
    
  }
  
  return(posts_df[-1, ])
}

# Calculate the posterior odds given the randomly generated data
random_post <- calc_posteriors_given_obs(random_obs)

# Put dataframe in long format
random_long <- gather(random_post, key = 'hyp', value = 'post', -n, -y, -next_lime)

# Theme for axis text and color vector
th  <- theme(axis.text = element_text(color = 'black'))
color_vector <- c('blue', 'red', 'darkgreen', 'darkorange', 'black')

# Graph the posteriors for each hypothesis with the random data
p1 <- ggplot(random_long, aes(n, post, col = hyp, group = hyp, shape = hyp)) + geom_line() + 
  geom_point() + xlab('Number of Observations') + ylab('Posterior') + 
  ggtitle('Posteriors for Random Data') + theme_economist(12) +  th + 
  scale_color_manual(values = color_vector)

# Graph the probability of the next candy being a lime with the random data
p2 <- ggplot(random_post, aes(n, next_lime)) + geom_line(col = 'red') + geom_point(col = 'red') + 
  xlab('Number of Observations') + ylab('Probability') + 
  ggtitle("Probability of Next Lime with Random Data") + 
  coord_cartesian(ylim = c(0, 1)) + scale_y_continuous(breaks = seq(0, 1, 0.1)) + 
  theme_economist(12) + th

print(p1)
print(p2)

# Create uncertainty columns for each hypothesis
random_unc <- random_post %>% mutate(h1_un = h1 - 0, 
                                     h2_un = h2 - 0, 
                                     h3_un = 1 - h3,
                                     h4_un = h4 - 0,
                                     h5_un = h5 - 0)

# Create a long format dataframe for plotting
random_unc_long <- random_unc %>% dplyr::select(n, h1_un:h5_un) %>%
  gather(key = 'hyp', value = 'unc', -n)

# Clean up the names for plotting
random_unc_long <- random_unc_long %>% 
  mutate(hyp = stringr::str_split_fixed(hyp, pattern = '_', n = 2)[, 1])

# Plot the uncertainties
ggplot(random_unc_long, aes(n, unc * 100, col = hyp, shape = hyp, group = hyp)) + 
  geom_line() + geom_point() + xlab('Number of Observations') + 
  ylab('Uncertainty for posterior (%)') + ggtitle('Uncertainty vs Obs for Random Data') + 
  scale_color_manual(values = color_vector) + theme_economist(12) + th 

# Create columns with reduction in uncertainty
random_reduc <- random_unc %>% mutate(h1_reduc = c(0, diff(h1_un)),
                                       h2_reduc = c(0, diff(h2_un)),
                                       h3_reduc = c(0, diff(h3_un)), 
                                       h4_reduc = c(0, diff(h4_un)),
                                       h5_reduc = c(0, diff(h5_un)))

# Put into long format
random_reduc_long <- random_reduc %>% dplyr::select(n, h1_reduc:h5_reduc) %>%
  gather(key = 'hyp', value = 'reduc', -n)

# Clean up the names for plotting 
random_reduc_long <- random_reduc_long %>% 
  mutate(hyp = stringr::str_split_fixed(hyp, pattern = '_', n = 2)[, 1])

# Plot the reductions in uncertainty
ggplot(random_reduc_long, aes(n, (reduc * 100), col = hyp, shape = hyp, group = hyp)) + 
  geom_line() + geom_point() + xlab('Number of Observations') + 
  ylab('Change in Uncertainty (%)') + ggtitle('Change in Uncertainty vs Obs') + 
  scale_color_manual(values = color_vector) + theme_economist(12) + th 


# Plot for the first 50 observations
ggplot(dplyr::filter(random_reduc_long, n <= 60), 
       aes(n, (reduc * 100), col = hyp, shape = hyp, group = hyp)) + 
  geom_line() + geom_point() + xlab('Number of Observations') + 
  ylab('Change in Uncertainty (%)') + ggtitle('Change in Uncertainty vs Obs') + 
  scale_color_manual(values = color_vector) + theme_economist(12) + th 


# 4. B. KMeans Implementation on Iris Dataset

# Read in Iris Data
iris_data <- read_csv('irisdata.csv')


kmeans <- function(k, iris_df, max_iter = 5) {
  
  # Select the features used for clustering
  iris_df <- dplyr::select(iris_df, petal_length, petal_width, species)
  
  # Choose initial cluster centers
  initial_indices <- sample(1:length(iris_df$species), size = k)
  cluster_centers <- data.matrix((iris_df[initial_indices, c(1,2)]))
  
  track_centers <- as.data.frame(matrix(ncol = 2))
  names(track_centers) <- c('petal_length', 'petal_width')
  
  index <- c()
  center_index <- c()
  
  total_errors <- c()
  
  # Iterate for the max iterations or until convergence
  for (n in 1:max_iter) {
    
    previous_cluster_centers <- cluster_centers
    classes <- c()
    
    track_centers <- rbind(track_centers, cluster_centers)
    
    # Iterate through the irises
    for (i in 1:nrow(iris_df)) {
      iris <- as.numeric((dplyr::select(iris_df, petal_length, petal_width))[i, ])
      distances <- c()
      
      # Iterate through the cluster centers 
      for (j in 1:k) {
        center <- cluster_centers[j, ]
        # Calculate the Euclidean distance between each point 
        # and the cluster center
        distance <- dist(matrix(data = c(center, iris), 
                                ncol = 2, byrow = TRUE), method = 'euclidean')
        
        distances <- c(distances, distance)
      }
      
      # The class is the cluster center with the minimum distance
      class <- which.min(distances)
      classes <- c(classes, class)
    }
    
    # Assign classes to data points
    iris_df$class <- classes
    track_errors <- c()
    
    # Loop to update all the cluster centers
    for (class_num in unique(iris_df$class)) {
      # Extract only those points assigned to the cluster
      class_df <- dplyr::filter(iris_df, class == class_num)
      # Calculate error associated with cluster center
      class_center <- cluster_centers[class_num, ]
      errors <- c(class_df$petal_length - class_center[1], 
                  class_df$petal_width - class_center[2])
      class_df$length_error <- class_df$petal_length - class_center[1] 
      class_df$width_error <- class_df$petal_width - class_center[2]
      class_df <- dplyr::mutate(class_df, total_error = length_error ^ 2 + 
                                 width_error ^ 2)
      
      # Keep track of the errors
      total_cluster_error <- sum(class_df$total_error)
      track_errors <- c(track_errors, total_cluster_error)
      
      # Update rule for cluster center
      cluster_centers[class_num, ] = c(mean(class_df$petal_length),
                                       mean(class_df$petal_width))
    }
    
    total_errors <- c(total_errors, sum(track_errors))
    index <- c(index, rep(n, k))
    center_index <- c(center_index, seq(1, k, by = 1))
    
    # Convergence condition
    if (all(previous_cluster_centers == cluster_centers)) {
      print('Convergence Achieved')
      track_centers <- track_centers[complete.cases(track_centers), ]
      track_centers$iter <- index
      track_centers$center <- center_index
      return(list('cluster_centers' = track_centers, 'errors' = total_errors))
      
      # If convergence not achieved track cluster centers and continue
    } else {
      previous_cluster_centers <- cluster_centers
    }
    
    
    
    print(sprintf('Iteration: %0.0f total error: %0.2f', n, 
                  sum(track_errors)))
    
    
  }
  # Return the position of clusters over iterations
  track_centers <- track_centers[complete.cases(track_centers), ]
  track_centers$iter <- index
  track_centers$center <- center_index
  return(list('cluster_centers' = track_centers, 'errors' = total_errors))
}

results <- kmeans(3, iris_data, max_iter = 5)

cluster_df <- results$cluster_centers
total_error <- results$errors

ggplot(cluster_df, aes(petal_length, petal_width, 
                       col = as.factor(center), shape = as.factor(iter))) + 
  geom_point() + 
  xlab('Petal Length (cm)') + 
  ylab('Petal Width (cm)') + 
  ggtitle('Petal Width vs Length by Iris Species') + theme_classic(12) + 
  scale_color_manual(values = c('firebrick', 'darkgreen', 'navy'))

ggplot(iris_data, aes(x = petal_length, y = petal_width, color = species)) + 
  geom_jitter() + xlab('Petal Length (cm)') + 
  ylab('Petal Width (cm)') + 
  ggtitle('Petal Width vs Length by Iris Species') + theme_classic(12) + 
  scale_color_manual(values = c('firebrick', 'darkgreen', 'navy'))
