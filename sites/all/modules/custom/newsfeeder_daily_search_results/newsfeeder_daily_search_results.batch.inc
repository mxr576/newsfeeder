<?php
/**
 * @file
 * Contains the batch operation callbacks' definitions.
 */

/**
 * Implements callback_batch_operation().
 *
 * Calculates the number of daily search results for each keyword
 * individually and aggregated.
 */
function _newsfeeder_daily_search_results_get_individual_and_aggregated_keyword_search_results_batch_operation($search_index, $from, $to, array $keywords, array $params = [], &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['limit_per_run'] = 5;
    $context['sandbox']['max'] = count($keywords) + 1;
  }
  $keywords_to_process = array_slice($keywords, $context['sandbox']['progress'], $context['sandbox']['limit_per_run']);
  foreach ($keywords_to_process as $keyword) {
    $context['results']['results']['keywords'][$keyword] = newsfeeder_daily_search_results_get_the_number_of_daily_search_results($search_index, $from, $to, $keyword, [], $params);
    $context['message'] = t("@keyword keyword's daily search results successfully processed.", ['@keyword' => $keyword]);
    $context['sandbox']['progress']++;
  }

  if ($context['sandbox']['progress'] + 1 == $context['sandbox']['max']) {
    $context['results']['results']['keywords-aggregated'] = newsfeeder_daily_search_results_get_the_number_of_daily_search_results($search_index, $from, $to, implode(' ', $keywords), [], $params);
    $context['message'] = t('Aggregated search result with all keywords successfully processed.');
    $context['sandbox']['progress']++;
  }
  $context['finished'] = ($context['sandbox']['progress'] / $context['sandbox']['max'] == 1);
}

/**
 * Implements callback_batch_operation().
 *
 * Calculates the number of daily search results for each filter
 * individually and aggregated.
 */
function _newsfeeder_daily_search_results_get_individual_and_aggregated_filter_search_results_batch_operation($search_index, $from, $to, array $filters, array $params = [], &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['limit_per_run'] = 5;
    $context['sandbox']['max'] = count($filters) + 1;
  }
  $filters_to_process = array_slice($filters, $context['sandbox']['progress'], $context['sandbox']['limit_per_run']);
  foreach ($filters_to_process as $filter => $value) {
    $context['results']['results']['filters'][$filter] = newsfeeder_daily_search_results_get_the_number_of_daily_search_results($search_index, $from, $to, '', [$filter => $value], $params);
    $terms = newsfeeder_hacks_get_name_of_the_taxonomy_terms([$value]);
    $context['message'] = t("@name filter's daily search results successfully processed.", ['@name' => array_shift($terms)]);
    $context['sandbox']['progress']++;
  }
  if ($context['sandbox']['progress'] + 1 == $context['sandbox']['max']) {
    $context['results']['results']['filters-aggregated'] = newsfeeder_daily_search_results_get_the_number_of_daily_search_results($search_index, $from, $to, '', $filters, $params);
    $context['message'] = t('Aggregated search result with all filters successfully processed.');
    $context['sandbox']['progress']++;
  }
  $context['finished'] = ($context['sandbox']['progress'] / $context['sandbox']['max'] == 1);
}

/**
 * Implements callback_batch_operation().
 *
 * Return the number of daily search result of that query, which contains all
 * keywords and filters parameters.
 */
function _newsfeeder_daily_search_results_get_aggregated_keywords_and_filters_search_results_batch_operation($search_index, $from, $to, array $keywords, array $filters, array $params = [], &$context) {
  $context['results']['results']['keywords-filters-aggregated'] = newsfeeder_daily_search_results_get_the_number_of_daily_search_results($search_index, $from, $to, implode(' ', $keywords), $filters, $params);
  $context['message'] = t('Aggregated search result with all keywords and filters successfully processed.');
  $context['finished'] = TRUE;
}

/**
 * Implements callback_batch_operation().
 *
 * Process the keyword query results for the csv export.
 */
function _newsfeeder_daily_search_results_export_merge_keyword_search_results_batch_operation(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['limit_per_run'] = 10;
    // +1, because of the aggregated keyword result.
    $context['sandbox']['max'] = count($context['results']['results']['keywords']) + 1;
    // Initialize the array, if it does not exists.
    if (!isset($context['results']['csv_content'])) {
      $context['results']['csv_content'] = [];
    }
  }
  $result_to_process = array_slice($context['results']['results']['keywords'], $context['sandbox']['progress'], $context['sandbox']['limit_per_run']);
  foreach ($result_to_process as $keyword) {
    $context['results']['csv_content'] = array_merge_recursive($context['results']['csv_content'], $keyword);
    $context['message'] = t("@count/@max keyword's result processed for the export.", ['@count' => $context['sandbox']['progress'], '@max' => $context['sandbox']['max'] - 1]);
    $context['sandbox']['progress']++;
  }

  // All keywords result processed, now process the aggregated
  // keyword result too.
  if ($context['sandbox']['progress'] + 1 == $context['sandbox']['max']) {
    $context['results']['csv_content'] = array_merge_recursive($context['results']['csv_content'], $context['results']['results']['keywords-aggregated']);
    $context['message'] = t("Aggregated keyword results processed for the export.");
    $context['sandbox']['progress']++;
  }

  if ($context['finished'] = ($context['sandbox']['progress'] / $context['sandbox']['max'] == 1)) {
    // Clean up unnecessary arrays.
    unset($context['results']['results']['keywords']);
    unset($context['results']['results']['keywords-aggregated']);
  }
}

/**
 * Implements callback_batch_operation().
 *
 * Process the filter query results for the csv export.
 */
function _newsfeeder_daily_search_results_export_merge_filter_search_results_batch_operation(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['limit_per_run'] = 10;
    // +1, because of the aggregated keyword result.
    $context['sandbox']['max'] = count($context['results']['results']['filters']) + 1;
    // Initialize the array, if it does not exists.
    if (!isset($context['results']['csv_content'])) {
      $context['results']['csv_content'] = [];
    }
  }
  $result_to_process = array_slice($context['results']['results']['filters'], $context['sandbox']['progress'], $context['sandbox']['limit_per_run']);
  foreach ($result_to_process as $filter) {
    $context['results']['csv_content'] = array_merge_recursive($context['results']['csv_content'], $filter);
    $context['message'] = t("@count/@max filter's result processed for the export.", ['@count' => $context['sandbox']['progress'], '@max' => $context['sandbox']['max'] - 1]);
    $context['sandbox']['progress']++;
  }
  // All filters result processed, now process the aggregated
  // filter result too.
  if ($context['sandbox']['progress'] + 1 == $context['sandbox']['max']) {
    $context['results']['csv_content'] = array_merge_recursive($context['results']['csv_content'], $context['results']['results']['filters-aggregated']);
    $context['message'] = t("Aggregated filter results processed for the export.");
    $context['sandbox']['progress']++;
  }

  if ($context['finished'] = ($context['sandbox']['progress'] / $context['sandbox']['max'] == 1)) {
    unset($context['results']['results']['filters']);
    unset($context['results']['results']['filters-aggregated']);
  }
}

/**
 * Implements callback_batch_operation().
 *
 * Process the aggregated keywords and filters query result for the csv export.
 */
function _newsfeeder_daily_search_results_export_merge_aggregated_keywords_and_filters_search_results_batch_operation(&$context) {
  if (!isset($context['results']['csv_content'])) {
    $context['results']['csv_content'] = [];
  }
  $context['results']['csv_content'] = array_merge_recursive($context['results']['csv_content'], $context['results']['results']['keywords-filters-aggregated']);
  $context['message'] = t("Aggregated keywords and filters result processed for the export.");
  unset($context['results']['results']['keywords-filters-aggregated']);
  $context['finished'] = TRUE;
}

/**
 * Implements callback_batch_operation().
 *
 * Generate formatted date columns for the csv file.
 */
function _newsfeeder_daily_search_results_export_generate_date_columns_batch_operation($date_format = 'm/d/Y', &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['iso8651_dates'] = array_keys($context['results']['csv_content']);
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['limit_per_run'] = 10;
    $context['sandbox']['max'] = count($context['results']['csv_content']);
    $context['sandbox']['formatted_dates'] = [];
  }
  $dates_to_process = array_slice($context['sandbox']['iso8651_dates'], $context['sandbox']['progress'], $context['sandbox']['limit_per_run']);
  foreach ($dates_to_process as $iso8651) {
    $ts = newsfeeder_solr_queries_parse_solr_iso_to_timestamp($iso8651);
    $context['sandbox']['formatted_dates'][$iso8651] = format_date($ts, 'custom', $date_format);
    $context['message'] = t('@count date successfully formatted for the export.', ['@count' => $context['sandbox']['progress']]);
    $context['sandbox']['progress']++;
  }
  if ($context['finished'] = $context['sandbox']['max'] == 0 || ($context['sandbox']['progress'] / $context['sandbox']['max'] == 1)) {
    $context['results']['csv_content'] = array_merge_recursive($context['sandbox']['formatted_dates'], $context['results']['csv_content']);
  }
}

/**
 * Implements callback_batch_operation().
 *
 * Write the results to a csv file.
 */
function _newsfeeder_daily_search_results_export_write_results_to_file_batch_operation($from, $to, array $keywords = [], array $filters = [], &$context) {
  if (empty($context['sandbox'])) {
    // Generate the header of the CSV file.
    $header = [];
    $header[] = t('Date @from - @to', ['@from' => format_date(strtotime($from), 'custom', 'm/d/Y'), '@to' => format_date(strtotime($to), 'custom', 'm/d/Y')]);
    // Add keywords, if any.
    if (!empty($keywords)) {
      array_walk($keywords, function ($keyword) use (&$header) {
        $header[] = $keyword;
      });
      $header[] = t('Keywords aggregated');
    }
    // Add the human readable name of the filters, if any.
    if (!empty($filters)) {
      array_walk($filters, function ($values, $field) use (&$header) {
        $terms = newsfeeder_hacks_get_name_of_the_taxonomy_terms([$values['value']]);
        $header[] = array_shift($terms);
      });
      $header[] = t('Filters aggregated');
    }

    if (!empty($keywords) && !empty($filters)) {
      $header[] = t('Keywords and filters aggregated');
    }
    // Generate unique file name and save the header to it.
    $file_name = file_create_filename('nf_demo_daily_search_stat_' . strtotime('now') . '.csv', NEWSFEEDER_DAILY_SEARCH_RESULTS_STATS_FOLDER);
    $context['results']['csv_file_name'] = format_string('nf_demo_daily_search_stat_@from_@to.csv', ['@from' => format_date(strtotime($from), 'custom', 'Ymd'), '@to' => format_date(strtotime($to), 'custom', 'Ymd')]);
    $context['results']['csv_file_path'] = file_unmanaged_save_data('', $file_name);
    $res = fopen($context['results']['csv_file_path'], 'w');
    fputcsv($res, $header);
    fclose($res);
    // Set some necessary data.
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['limit_per_run'] = 10;
    $context['sandbox']['max'] = count($context['results']['csv_content']);
  }

  $lines_to_write = array_slice($context['results']['csv_content'], $context['sandbox']['progress'], $context['sandbox']['limit_per_run']);
  $res = fopen($context['results']['csv_file_path'], 'a');
  foreach ($lines_to_write as $line) {
    fputcsv($res, $line);
    $context['message'] = t('@progress/@max line successfully written to the export file.', ['@progress' => $context['sandbox']['progress'], '@max' => $context['sandbox']['max']]);
    $context['sandbox']['progress']++;
  }
  fclose($res);

  $context['finished'] = $context['sandbox']['max'] == 0 || ($context['sandbox']['progress'] / $context['sandbox']['max'] == 1);
}

/**
 * Implements callback_batch_finished().
 */
function _newsfeeder_daily_search_results_get_individual_and_aggregated_keyword_search_results_batch_finished($success, $results, $operations) {
  if ($success) {
    // Send the generated file to the user.
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $results['csv_file_name']);
    // Send the file to the user.
    @readfile(file_create_url($results['csv_file_path']));
    drupal_exit();
  }
  else {
    drupal_set_message(t('Something went wrong, please try again!'), 'error');
  }
}
