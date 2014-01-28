<?php

/**
 * @file
 * Based on context/plugins/context_reaction_breadcrumb together with patch
 * https://drupal.org/node/835090#comment-7948081
 */

/**
 * Expose breadcrumbs as context reactions.
 */
class context_menutrail_breadcrumb_reaction extends context_menutrail_menu_reaction {
  /**
   * Overrides set_active_trail_from_link to set the breadcrumb instead of the menu path.
   */
  function set_active_trail_from_link($item) {
    $result = db_select('menu_links')
      ->fields('menu_links', array('p1', 'p2', 'p3', 'p4', 'p5', 'p6', 'p7', 'p8'))
      ->condition('hidden', 0)
      ->condition('link_path', $item['link_path'])
      ->execute();
    while ($parents = $result->fetchAssoc()) {
      $set = FALSE;
      foreach (array_filter($parents) as $plid) {
        $parent = menu_link_load($plid);
        if ($parent && $parent['access'] && empty($parent['hidden']) && !empty($parent['title'])) {
          $set = TRUE;
          $breadcrumb[] = l($parent['title'], $parent['href']);
        }
      }
      // Only set the breadcrumb if one or more links were added to the
      // trail. If not, continue iterating through possible menu links.
      if ($set) {
        drupal_set_breadcrumb($breadcrumb);
        break;
      }
    }
  }
}
